/* 文件: rtos.h | 编码: UTF-8 | 每行代码附中文注释 */
#ifndef __RTOS_H                                        /* 防止头文件重复包含 */
#define __RTOS_H                                        /* 定义包含保护宏 */

#include <stdint.h>                                     /* 包含 C 标准库头文件 */

#define MAX_TASKS               8                       /* 定义宏 */
#define portMAX_DELAY           (0xFFFFFFFFu)           /* 定义宏 */

typedef struct list_item                                /* 类型定义 */
{                                                       /* 代码块开始 */
    struct list_item *pxNext;                           /* 执行语句 */
    void *pvOwner;                                      /* 函数定义或声明 */
} ListItem_t;                                           /* 代码块结束 */

/* 任务状态 */
typedef enum {                                          /* 类型定义 */
    TASK_READY = 0,                                     /* 列表续行 */
    TASK_DELAY,                                         /* 列表续行 */
    TASK_BLOCKED,                                       /* 列表续行 */
    TASK_RUNNING                                        /* 源代码 */
} task_state_t;                                         /* 代码块结束 */

/* 任务控制块 */
typedef struct tcb {                                    /* 类型定义 */
    uint32_t *sp;                                       /* 32位无符号变量/函数 */
    uint32_t delay_ticks;                               /* 32位无符号变量/函数 */
    task_state_t state;                                 /* 执行语句 */
    uint8_t priority;                                   /* 8位无符号变量/函数 */
    int32_t block_result;                               /* 32位有符号变量/函数 */
    ListItem_t xStateListItem;                          /* 执行语句 */
    ListItem_t xEventListItem;                          /* 执行语句 */
    ListItem_t **ppEventListHead;                       /* 执行语句 */
} tcb_t;                                                /* 代码块结束 */

/* 信号量 */
typedef struct semaphore                                /* 类型定义 */
{                                                       /* 代码块开始 */
    int32_t count;                                      /* 32位有符号变量/函数 */
    int32_t maxCount;                                   /* 32位有符号变量/函数 */
    ListItem_t *pxWaitingListHead;                      /* 执行语句 */
} Semaphore_t;                                          /* 代码块结束 */

/* 互斥量（支持简单递归） */
typedef struct mutex                                    /* 类型定义 */
{                                                       /* 代码块开始 */
    int32_t recursive_count;                            /* 32位有符号变量/函数 */
    ListItem_t *pxWaitingListHead;                      /* 执行语句 */
    tcb_t *owner;                                       /* 执行语句 */
} Mutex_t;                                              /* 代码块结束 */

/* 队列 */
typedef struct queue                                    /* 类型定义 */
{                                                       /* 代码块开始 */
    void *pvBuffer;                                     /* 函数定义或声明 */
    uint32_t itemSize;                                  /* 32位无符号变量/函数 */
    uint32_t maxItems;                                  /* 32位无符号变量/函数 */
    uint32_t currentItems;                              /* 32位无符号变量/函数 */
    uint32_t headIndex;                                 /* 32位无符号变量/函数 */
    uint32_t tailIndex;                                 /* 32位无符号变量/函数 */
    ListItem_t *pxSendWaitingListHead;                  /* 执行语句 */
    ListItem_t *pxReceiveWaitingListHead;               /* 执行语句 */
} Queue_t;                                              /* 代码块结束 */

typedef void (*task_func_t)(void);                      /* 类型定义 */

extern tcb_t *current_tcb;                              /* 外部符号声明 */

/* stack_size 单位为字（uint32_t） */
void task_create(tcb_t *tcb, task_func_t task, uint32_t *stack, uint32_t stack_size, uint8_t priority);  /* 函数定义或声明 */
void start_first_task(void);                            /* 函数定义或声明 */
void task_switch(void);                                 /* 函数定义或声明 */
void schedule(void);                                    /* 函数定义或声明 */
void task_delay(uint32_t ticks);                        /* 函数定义或声明 */
void systick_init(void);                                /* 函数定义或声明 */
void ready_list_add(tcb_t *tcb);                        /* 函数定义或声明 */
void ready_list_remove(tcb_t *tcb);                     /* 函数定义或声明 */

void xSemaphoreCreateCounting(Semaphore_t *sem, int32_t maxCount, int32_t initialCount);  /* 函数定义或声明 */
int32_t xSemaphoreTake(Semaphore_t *sem, uint32_t waitTicks);  /* 32位有符号变量/函数 */
int32_t xSemaphoreGive(Semaphore_t *sem);               /* 32位有符号变量/函数 */

void xMutexCreate(Mutex_t *mutex);                      /* 函数定义或声明 */
int32_t xMutexTake(Mutex_t *mutex, uint32_t waitTicks);  /* 32位有符号变量/函数 */
int32_t xMutexGive(Mutex_t *mutex);                     /* 32位有符号变量/函数 */

int32_t xQueueCreate(Queue_t *queue, void *buffer, uint32_t itemSize, uint32_t maxItems);  /* 32位有符号变量/函数 */
int32_t xQueueSend(Queue_t *queue, const void *item, uint32_t waitTicks);  /* 32位有符号变量/函数 */
int32_t xQueueReceive(Queue_t *queue, void *item, uint32_t waitTicks);  /* 32位有符号变量/函数 */


#endif                                                  /* 条件编译结束 */
