#include "rtos.h"
#include "stm32f4xx.h"
#include <stddef.h>
#include <string.h>

/* 就绪 / 延时链表 */
ListItem_t *pxReadyListHead   = NULL;
ListItem_t *pxDelayedListHead = NULL;

/* 所有任务（调试用） */
tcb_t *task_list[MAX_TASKS];
uint8_t task_count = 0;

tcb_t *current_tcb = NULL;

/* ======================== 链表工具 ======================== */

static void list_insert_head(ListItem_t **ppHead, ListItem_t *item)
{
    item->pxNext = *ppHead;
    *ppHead = item;
}

static void list_remove(ListItem_t **ppHead, ListItem_t *item)
{
    ListItem_t *prev = NULL;
    ListItem_t *cur  = *ppHead;

    while (cur != NULL)
    {
        if (cur == item)
        {
            if (prev == NULL)
            {
                *ppHead = cur->pxNext;
            }
            else
            {
                prev->pxNext = cur->pxNext;
            }
            item->pxNext = NULL;
            return;
        }
        prev = cur;
        cur  = cur->pxNext;
    }
}

static void delayed_list_add(tcb_t *tcb, uint32_t ticks)
{
    tcb->delay_ticks = ticks;
    list_insert_head(&pxDelayedListHead, &tcb->xStateListItem);
}

static void delayed_list_remove(tcb_t *tcb)
{
    list_remove(&pxDelayedListHead, &tcb->xStateListItem);
}

/* 加入事件等待链表；若 waitTicks 有限则同时挂到延时链表做超时 */
static void task_block_on_event(ListItem_t **ppEventHead, uint32_t waitTicks)
{
    ready_list_remove(current_tcb);

    current_tcb->xEventListItem.pvOwner = current_tcb;
    list_insert_head(ppEventHead, &current_tcb->xEventListItem);
    current_tcb->ppEventListHead = ppEventHead;
    current_tcb->block_result    = 0;
    current_tcb->state           = TASK_BLOCKED;

    if (waitTicks != portMAX_DELAY)
    {
        delayed_list_add(current_tcb, waitTicks);
    }
    else
    {
        current_tcb->delay_ticks = 0;
    }
}

/* 从事件链表摘掉，并从延时链表摘掉（若在上面） */
static void task_unblock_from_event(tcb_t *tcb, int32_t result)
{
    if (tcb->ppEventListHead != NULL)
    {
        list_remove(tcb->ppEventListHead, &tcb->xEventListItem);
        tcb->ppEventListHead = NULL;
    }

    /* 可能同时挂在延时链表（带超时的阻塞） */
    delayed_list_remove(tcb);

    tcb->delay_ticks  = 0;
    tcb->block_result = result;
    ready_list_add(tcb);
}

/* 等待直到离开阻塞/延时状态（被切走再切回后退出） */
static void task_wait_until_ready(void)
{
    task_switch();

    while (current_tcb->state == TASK_DELAY || current_tcb->state == TASK_BLOCKED)
    {
        /* 忙等直到被调度回来且状态已就绪 */
    }
}

/* ======================== 任务创建 / 切换 ======================== */

void task_create(tcb_t *tcb, task_func_t task, uint32_t *stack, uint32_t stack_size, uint8_t priority)
{
    /* 栈顶 8 字节对齐（Cortex-M 要求） */
    uint32_t *sp = stack + stack_size;
    sp = (uint32_t *)((uint32_t)sp & ~0x7u);

    /* 硬件自动压栈的 8 个寄存器 */
    *(--sp) = 0x01000000u;            /* xPSR Thumb */
    *(--sp) = (uint32_t)task;         /* PC */
    *(--sp) = 0xFFFFFFFDu;            /* LR = EXC_RETURN thread/PSP */
    *(--sp) = 0x12121212u;            /* R12 */
    *(--sp) = 0x03030303u;            /* R3 */
    *(--sp) = 0x02020202u;            /* R2 */
    *(--sp) = 0x01010101u;            /* R1 */
    *(--sp) = 0x00000000u;            /* R0 */

    /* 软件保存 R4~R11 */
    *(--sp) = 0x11111111u;
    *(--sp) = 0x10101010u;
    *(--sp) = 0x09090909u;
    *(--sp) = 0x08080808u;
    *(--sp) = 0x07070707u;
    *(--sp) = 0x06060606u;
    *(--sp) = 0x05050505u;
    *(--sp) = 0x04040404u;

    tcb->sp              = sp;
    tcb->delay_ticks     = 0;
    tcb->state           = TASK_READY;
    tcb->priority        = priority;
    tcb->block_result    = 1;
    tcb->ppEventListHead = NULL;

    tcb->xStateListItem.pxNext  = NULL;
    tcb->xStateListItem.pvOwner = tcb;
    tcb->xEventListItem.pxNext  = NULL;
    tcb->xEventListItem.pvOwner = tcb;

    ready_list_add(tcb);

    if (task_count < MAX_TASKS)
    {
        task_list[task_count++] = tcb;
    }
}

__attribute__((naked)) void start_first_task(void)
{
    __asm volatile
    (
        "ldr r0, =current_tcb       \n"
        "ldr r0, [r0]               \n"
        "ldr r0, [r0]               \n"
        "msr psp, r0                \n"
        "mov r0, #2                 \n"
        "msr control, r0            \n"
        "isb                        \n"

        "pop {r4-r11}               \n"
        "pop {r0-r3}                \n"
        "pop {r12}                  \n"
        "pop {lr}                   \n"
        "pop {r1}                   \n" /* PC */
        "pop {r2}                   \n" /* xPSR */
        "msr apsr_nzcvq, r2         \n"
        "bx  r1                     \n"
    );
}

void task_switch(void)
{
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

__attribute__((naked)) void PendSV_Handler(void)
{
    __asm volatile
    (
        "mrs r0, psp                    \n"
        "stmdb r0!, {r4-r11}            \n"

        "ldr r1, =current_tcb           \n"
        "ldr r2, [r1]                   \n"
        "str r0, [r2]                   \n"

        "push {lr}                      \n"
        "bl schedule                    \n"
        "pop {lr}                       \n"

        "ldr r1, =current_tcb           \n"
        "ldr r2, [r1]                   \n"
        "ldr r0, [r2]                   \n"
        "ldmia r0!, {r4-r11}            \n"
        "msr psp, r0                    \n"

        "bx lr                          \n"
    );
}

void schedule(void)
{
    static tcb_t *last_selected = NULL;

    /* 防止 SysTick 抢占 PendSV 时改链表 */
    __disable_irq();

    if (pxReadyListHead == NULL)
    {
        __enable_irq();
        return;
    }

    ListItem_t *item;
    uint8_t max_pri = 0;

    /* 找最高优先级数值 */
    item = pxReadyListHead;
    while (item != NULL)
    {
        tcb_t *t = (tcb_t *)item->pvOwner;
        if (t->priority > max_pri)
        {
            max_pri = t->priority;
        }
        item = item->pxNext;
    }

    /*
     * 同优先级轮转：
     * 在就绪链表中找 priority==max_pri 的任务，
     * 优先选 last_selected 之后的下一个，没有则绕回第一个。
     */
    tcb_t *first_same = NULL;
    tcb_t *chosen     = NULL;
    uint8_t seen_last = 0;

    item = pxReadyListHead;
    while (item != NULL)
    {
        tcb_t *t = (tcb_t *)item->pvOwner;
        if (t->priority == max_pri)
        {
            if (first_same == NULL)
            {
                first_same = t;
            }
            if (seen_last && chosen == NULL)
            {
                chosen = t;
            }
            if (t == last_selected)
            {
                seen_last = 1;
            }
        }
        item = item->pxNext;
    }

    if (chosen == NULL)
    {
        chosen = first_same;
    }

    last_selected      = chosen;
    current_tcb        = chosen;
    current_tcb->state = TASK_RUNNING;

    __enable_irq();
}

/* ======================== 延时 / Tick ======================== */

void task_delay(uint32_t ticks)
{
    if (ticks == 0)
    {
        return;
    }

    __disable_irq();

    ready_list_remove(current_tcb);
    current_tcb->state = TASK_DELAY;
    delayed_list_add(current_tcb, ticks);

    __enable_irq();

    task_wait_until_ready();
}

void systick_init(void)
{
    /* 168MHz HSI：168000 周期 = 1ms */
    SysTick_Config(168000);
    /* 保证 SysTick 优先级高于 PendSV，但临界区用关中断保护链表 */
    NVIC_SetPriority(SysTick_IRQn, 0x0F);
}

void SysTick_Handler(void)
{
    ListItem_t *item = pxDelayedListHead;

    while (item != NULL)
    {
        /* 必须先保存 next：ready_list_add 会改写 xStateListItem.pxNext */
        ListItem_t *next = item->pxNext;
        tcb_t *tcb = (tcb_t *)item->pvOwner;

        if (tcb->delay_ticks > 0)
        {
            tcb->delay_ticks--;
        }

        if (tcb->delay_ticks == 0)
        {
            if (tcb->state == TASK_DELAY)
            {
                delayed_list_remove(tcb);
                ready_list_add(tcb);
            }
            else if (tcb->state == TASK_BLOCKED)
            {
                /* 等待同步对象超时 */
                task_unblock_from_event(tcb, 0);
            }
        }

        item = next;
    }

    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

/* ======================== 就绪链表 ======================== */

void ready_list_add(tcb_t *tcb)
{
    if (tcb == NULL)
    {
        return;
    }

    list_insert_head(&pxReadyListHead, &tcb->xStateListItem);
    tcb->state = TASK_READY;
}

void ready_list_remove(tcb_t *tcb)
{
    if (tcb == NULL)
    {
        return;
    }

    list_remove(&pxReadyListHead, &tcb->xStateListItem);
}

/* ======================== 信号量 ======================== */

void xSemaphoreCreateCounting(Semaphore_t *sem, int32_t maxCount, int32_t initialCount)
{
    if (sem == NULL || maxCount <= 0)
    {
        return;
    }

    if (initialCount < 0)
    {
        initialCount = 0;
    }
    if (initialCount > maxCount)
    {
        initialCount = maxCount;
    }

    sem->maxCount          = maxCount;
    sem->count             = initialCount;
    sem->pxWaitingListHead = NULL;
}

int32_t xSemaphoreTake(Semaphore_t *sem, uint32_t waitTicks)
{
    if (sem == NULL)
    {
        return 0;
    }

    for (;;)
    {
        __disable_irq();

        if (sem->count > 0)
        {
            sem->count--;
            __enable_irq();
            return 1;
        }

        if (waitTicks == 0)
        {
            __enable_irq();
            return 0;
        }

        task_block_on_event(&sem->pxWaitingListHead, waitTicks);
        __enable_irq();

        task_wait_until_ready();

        if (current_tcb->block_result == 0)
        {
            return 0; /* 超时 */
        }
        /* 被 Give 唤醒后重试 Take，正确扣减 count */
    }
}

int32_t xSemaphoreGive(Semaphore_t *sem)
{
    if (sem == NULL)
    {
        return 0;
    }

    __disable_irq();

    if (sem->count >= sem->maxCount)
    {
        __enable_irq();
        return 0;
    }

    sem->count++;

    if (sem->pxWaitingListHead != NULL)
    {
        tcb_t *tcb = (tcb_t *)sem->pxWaitingListHead->pvOwner;
        task_unblock_from_event(tcb, 1);
    }

    __enable_irq();
    task_switch();
    return 1;
}

/* ======================== 互斥量 ======================== */

void xMutexCreate(Mutex_t *mutex)
{
    if (mutex == NULL)
    {
        return;
    }

    mutex->recursive_count = 0;
    mutex->pxWaitingListHead = NULL;
    mutex->owner = NULL;
}

int32_t xMutexTake(Mutex_t *mutex, uint32_t waitTicks)
{
    if (mutex == NULL)
    {
        return 0;
    }

    for (;;)
    {
        __disable_irq();

        if (mutex->recursive_count == 0)
        {
            mutex->recursive_count = 1;
            mutex->owner = current_tcb;
            __enable_irq();
            return 1;
        }

        if (mutex->owner == current_tcb)
        {
            mutex->recursive_count++;
            __enable_irq();
            return 1;
        }

        if (waitTicks == 0)
        {
            __enable_irq();
            return 0;
        }

        task_block_on_event(&mutex->pxWaitingListHead, waitTicks);
        __enable_irq();

        task_wait_until_ready();

        if (current_tcb->block_result == 0)
        {
            return 0;
        }
    }
}

int32_t xMutexGive(Mutex_t *mutex)
{
    if (mutex == NULL || mutex->owner != current_tcb)
    {
        return 0;
    }

    __disable_irq();

    if (mutex->recursive_count > 1)
    {
        mutex->recursive_count--;
        __enable_irq();
        return 1;
    }

    mutex->recursive_count = 0;
    mutex->owner = NULL;

    if (mutex->pxWaitingListHead != NULL)
    {
        tcb_t *tcb = (tcb_t *)mutex->pxWaitingListHead->pvOwner;
        task_unblock_from_event(tcb, 1);
    }

    __enable_irq();
    task_switch();
    return 1;
}

/* ======================== 队列 ======================== */

int32_t xQueueCreate(Queue_t *queue, void *buffer, uint32_t itemSize, uint32_t maxItems)
{
    if (queue == NULL || buffer == NULL || itemSize == 0 || maxItems == 0)
    {
        return 0;
    }

    queue->pvBuffer                  = buffer;
    queue->itemSize                  = itemSize;
    queue->maxItems                  = maxItems;
    queue->currentItems              = 0;
    queue->headIndex                 = 0;
    queue->tailIndex                 = 0;
    queue->pxSendWaitingListHead     = NULL;
    queue->pxReceiveWaitingListHead  = NULL;

    return 1;
}

int32_t xQueueSend(Queue_t *queue, const void *item, uint32_t waitTicks)
{
    if (queue == NULL || item == NULL)
    {
        return 0;
    }

    for (;;)
    {
        __disable_irq();

        if (queue->currentItems < queue->maxItems)
        {
            uint8_t *dest = (uint8_t *)queue->pvBuffer + (queue->tailIndex * queue->itemSize);
            memcpy(dest, item, queue->itemSize);

            queue->tailIndex = (queue->tailIndex + 1u) % queue->maxItems;
            queue->currentItems++;

            if (queue->pxReceiveWaitingListHead != NULL)
            {
                tcb_t *tcb = (tcb_t *)queue->pxReceiveWaitingListHead->pvOwner;
                task_unblock_from_event(tcb, 1);
            }

            __enable_irq();
            task_switch();
            return 1;
        }

        if (waitTicks == 0)
        {
            __enable_irq();
            return 0;
        }

        task_block_on_event(&queue->pxSendWaitingListHead, waitTicks);
        __enable_irq();

        task_wait_until_ready();

        if (current_tcb->block_result == 0)
        {
            return 0;
        }
        /* 有空位后重试发送 */
    }
}

int32_t xQueueReceive(Queue_t *queue, void *item, uint32_t waitTicks)
{
    if (queue == NULL || item == NULL)
    {
        return 0;
    }

    for (;;)
    {
        __disable_irq();

        if (queue->currentItems > 0)
        {
            uint8_t *src = (uint8_t *)queue->pvBuffer + (queue->headIndex * queue->itemSize);
            memcpy(item, src, queue->itemSize);

            queue->headIndex = (queue->headIndex + 1u) % queue->maxItems;
            queue->currentItems--;

            if (queue->pxSendWaitingListHead != NULL)
            {
                tcb_t *tcb = (tcb_t *)queue->pxSendWaitingListHead->pvOwner;
                task_unblock_from_event(tcb, 1);
            }

            __enable_irq();
            task_switch();
            return 1;
        }

        if (waitTicks == 0)
        {
            __enable_irq();
            return 0;
        }

        task_block_on_event(&queue->pxReceiveWaitingListHead, waitTicks);
        __enable_irq();

        task_wait_until_ready();

        if (current_tcb->block_result == 0)
        {
            return 0;
        }
        /* 有数据后重试接收 */
    }
}
