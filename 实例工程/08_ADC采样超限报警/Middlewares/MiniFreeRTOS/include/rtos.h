#ifndef __RTOS_H
#define __RTOS_H

#include <stdint.h>

#define MAX_TASKS               8
#define portMAX_DELAY           (0xFFFFFFFFu)

typedef struct list_item
{
    struct list_item *pxNext;     /* 指向下一个节点 */
    void *pvOwner;                /* 指向所属的 TCB */
} ListItem_t;

/* 任务状态 */
typedef enum {
    TASK_READY = 0,
    TASK_DELAY,                   /* 纯延时 */
    TASK_BLOCKED,                 /* 等待信号量/互斥/队列（可带超时） */
    TASK_RUNNING
} task_state_t;

/* 任务控制块 */
typedef struct tcb {
    uint32_t *sp;                 /* 必须是第一个成员 */
    uint32_t delay_ticks;
    task_state_t state;
    uint8_t priority;             /* 数字越大优先级越高 */
    int32_t block_result;         /* 1=被事件唤醒成功, 0=超时 */
    ListItem_t xStateListItem;    /* 就绪链表 或 延时链表 */
    ListItem_t xEventListItem;    /* 同步对象等待链表 */
    ListItem_t **ppEventListHead; /* 当前所在等待链表头指针的地址，不在链表则为 NULL */
} tcb_t;

/* 信号量 */
typedef struct semaphore
{
    int32_t count;
    int32_t maxCount;
    ListItem_t *pxWaitingListHead;
} Semaphore_t;

/* 互斥量（支持简单递归） */
typedef struct mutex
{
    int32_t recursive_count;      /* 0=空闲, >0=嵌套次数 */
    ListItem_t *pxWaitingListHead;
    tcb_t *owner;
} Mutex_t;

/* 队列 */
typedef struct queue
{
    void *pvBuffer;
    uint32_t itemSize;
    uint32_t maxItems;
    uint32_t currentItems;
    uint32_t headIndex;
    uint32_t tailIndex;
    ListItem_t *pxSendWaitingListHead;
    ListItem_t *pxReceiveWaitingListHead;
} Queue_t;

typedef void (*task_func_t)(void);

extern tcb_t *current_tcb;

/* stack_size 单位为字（uint32_t） */
void task_create(tcb_t *tcb, task_func_t task, uint32_t *stack, uint32_t stack_size, uint8_t priority);
void start_first_task(void);
void task_switch(void);
void schedule(void);
void task_delay(uint32_t ticks);
void systick_init(void);
void ready_list_add(tcb_t *tcb);
void ready_list_remove(tcb_t *tcb);

void xSemaphoreCreateCounting(Semaphore_t *sem, int32_t maxCount, int32_t initialCount);
int32_t xSemaphoreTake(Semaphore_t *sem, uint32_t waitTicks);
int32_t xSemaphoreGive(Semaphore_t *sem);

void xMutexCreate(Mutex_t *mutex);
int32_t xMutexTake(Mutex_t *mutex, uint32_t waitTicks);
int32_t xMutexGive(Mutex_t *mutex);

int32_t xQueueCreate(Queue_t *queue, void *buffer, uint32_t itemSize, uint32_t maxItems);
int32_t xQueueSend(Queue_t *queue, const void *item, uint32_t waitTicks);
int32_t xQueueReceive(Queue_t *queue, void *item, uint32_t waitTicks);


#endif
