/**
 * @file system.c
 * @brief TODO.
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include "stm32f7xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "config.h"
#include "nvicconf.h"
#include "debugio.h"
#include "led.h"
#include "uart1.h"
#include "uart2.h"
//#include "can1.h"
#include "platform.h"
//#include "udpserver.h"
//#include "imu.h"
#include "system.h"

static const TickType_t SYS_UPDATE_FREQ = M2T(1000);

static xSemaphoreHandle system_ready_mutex = NULL;

static StaticTask_t idle_task_tcb;
static StackType_t idle_task_stack[IDLE_TASK_STACKSIZE];

static StaticTask_t sys_task_tcb;
static StackType_t sys_task_stack[SYSTEM_TASK_STACKSIZE];

static StaticSemaphore_t system_ready_mutex_handle;

static bool is_init = false;

static void signal_ready_to_start(void)
{
    xSemaphoreGive(system_ready_mutex);
}

static void system_init(void)
{
    if(is_init == false)
    {
        system_ready_mutex = xSemaphoreCreateMutexStatic(
                &system_ready_mutex_handle);

        xSemaphoreTake(system_ready_mutex, portMAX_DELAY);

        is_init = true;
    }

    debug_puts(SYSTEM_TASK_NAME" started");
}

#ifdef BUILD_TYPE_DEBUG
static void debug_output_runtime_stats(void)
{
    unsigned long total_run_time = 0;
    static TaskStatus_t status_array[TASK_COUNT];

    volatile UBaseType_t actual_cnt = uxTaskGetNumberOfTasks();

    configASSERT(((UBaseType_t) TASK_COUNT) >= actual_cnt);

    actual_cnt = uxTaskGetSystemState(
            &status_array[0],
            actual_cnt,
            &total_run_time);

    UBaseType_t idx;
    for(idx = 0; idx < actual_cnt; idx += 1)
    {
        debug_printf(
                "'%s' - %hu\r\n",
                status_array[idx].pcTaskName,
                status_array[idx].usStackHighWaterMark);
    }
    debug_puts("");
}
#endif

static void system_task(void *params)
{
    (void) params;
    TickType_t last_wake_time;

    led_init();
    led_on(LED_SYSTEM_STATUS);

    uart1_init(UART1_BAUDRATE);

    //can1_init();

    system_init();

    //udpserver_start();

    //imu_start();

    signal_ready_to_start();

    last_wake_time = xTaskGetTickCount();

    while(1)
    {
        vTaskDelayUntil(&last_wake_time, SYS_UPDATE_FREQ);

        led_toggle(LED_SYSTEM_STATUS);
        led_off(LED_UART2_STATUS);

#ifdef BUILD_TYPE_DEBUG
        debug_output_runtime_stats();
#endif
    }

    // should never get here
    platform_error_handler();
}

void system_start(void)
{
    (void) xTaskCreateStatic(
            &system_task,
            SYSTEM_TASK_NAME,
            SYSTEM_TASK_STACKSIZE,
            NULL,
            SYSTEM_TASK_PRI,
            &sys_task_stack[0],
            &sys_task_tcb);
}

void system_wait_for_start(void)
{
    while(is_init == false)
    {
        vTaskDelay(5);
    }

    xSemaphoreTake(system_ready_mutex, portMAX_DELAY);
    xSemaphoreGive(system_ready_mutex);
}

void vApplicationGetIdleTaskMemory(
        StaticTask_t **ppxIdleTaskTCBBuffer,
        StackType_t **ppxIdleTaskStackBuffer,
        uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &idle_task_tcb;
    *ppxIdleTaskStackBuffer = &idle_task_stack[0];
    *pulIdleTaskStackSize = (uint32_t) IDLE_TASK_STACKSIZE;
}
