#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "stm32f10x.h"

/* 基本配置 */
#define configUSE_PREEMPTION            1       /* 抢占式调度 */
#define configUSE_IDLE_HOOK             1       /* 使用空闲钩子 */
#define configUSE_TICK_HOOK             0       /* 不使用时钟钩子 */
#define configCPU_CLOCK_HZ              72000000 /* 系统时钟72MHz */
#define configTICK_RATE_HZ              1000    /* tick频率1000Hz = 1ms */
#define configMAX_PRIORITIES            5       /* 优先级数量0~4 */
#define configMINIMAL_STACK_SIZE        128     /* 空闲任务栈大小(word) */
#define configTOTAL_HEAP_SIZE           (10*1024) /* 堆大小10KB */
#define configMAX_TASK_NAME_LEN         16      /* 任务名最大长度 */
#define configUSE_16_BIT_TICKS          0       /* 32位tick计数 */
#define configIDLE_SHOULD_YIELD         1       /* 空闲任务让出CPU */

/* 函数选项 */
#define configUSE_MUTEXES               1       /* 使用互斥信号量 */
#define configUSE_RECURSIVE_MUTEXES     0       /* 不使用递归互斥量 */
#define configUSE_COUNTING_SEMAPHORES   1       /* 使用计数信号量 */
#define configUSE_ALTERNATIVE_API       0       /* 不使用旧API */
#define configUSE_TIMERS                1       /* 使用软件定时器 */
#define configTIMER_TASK_PRIORITY       3       /* 定时器服务任务优先级 */
#define configTIMER_QUEUE_LENGTH        10      /* 定时器命令队列长度 */
#define configTIMER_TASK_STACK_DEPTH    256     /* 定时器服务任务栈大小(word) */
#define configUSE_CO_ROUTINES           0       /* 不使用协程 */
#define configUSE_EVENT_GROUPS          1       /* 使用事件组 */

/* 钩子函数 */
#define configCHECK_FOR_STACK_OVERFLOW  0       /* 不检查栈溢出 */
#define configUSE_MALLOC_FAILED_HOOK    0       /* 不使用内存分配失败钩子 */

/* 运行时统计 */
#define configGENERATE_RUN_TIME_STATS   0       /* 不生成运行时统计 */
#define configUSE_TRACE_FACILITY        0       /* 不使用追踪功能 */

/* 中断优先级配置 - Cortex-M3 */
#define configPRIO_BITS                 4       /* 4位优先级(16级) */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY        15 /* 最低优先级 */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY    5  /* 系统调用最高优先级 */
#define configKERNEL_INTERRUPT_PRIORITY (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))
#define configMAX_SYSCALL_INTERRUPT_PRIORITY (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

/* 可选函数 */
#define INCLUDE_vTaskPrioritySet        1
#define INCLUDE_uxTaskPriorityGet       1
#define INCLUDE_vTaskDelete             1
#define INCLUDE_vTaskCleanUpResources   0
#define INCLUDE_vTaskSuspend            1
#define INCLUDE_vTaskDelayUntil         1
#define INCLUDE_vTaskDelay              1
#define INCLUDE_xTaskGetSchedulerState  1
#define INCLUDE_xTimerPendFunctionCall  0
#define INCLUDE_uxTaskGetStackHighWaterMark  1

/* 中断服务函数 - FreeRTOS提供，这里定义为弱别名 */
#define vPortSVCHandler     SVC_Handler
#define xPortPendSVHandler  PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */
