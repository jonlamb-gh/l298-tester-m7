/**
 * @file control.c
 * @brief TODO.
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include "stm32f7xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "config.h"
#include "nvicconf.h"
#include "debug.h"
#include "led.h"
#include "system.h"
#include "control.h"

static StaticTask_t control_task_tcb;
static StackType_t control_task_stack[CONTROL_TASK_STACKSIZE];

static bool is_init = false;

static void control_task(
        void * const params)
{
    (void) params;

    system_wait_for_start();

    led_off(LED_GREEN);

    debug_puts(CONTROL_TASK_NAME" started");

    // TODO - testing
    TickType_t last_wake_time = xTaskGetTickCount();

    // TODO - PB0, green LED, and TIMER_D_PWM1 / TIM3_CH3

    while(1)
    {
        vTaskDelayUntil(&last_wake_time, M2T(500));

        led_toggle(LED_GREEN);
    }
}

static void control_init(void)
{
    if(is_init == false)
    {
        (void) xTaskCreateStatic(
                &control_task,
                CONTROL_TASK_NAME,
                CONTROL_TASK_STACKSIZE,
                NULL,
                CONTROL_TASK_PRI,
                &control_task_stack[0],
                &control_task_tcb);

        is_init = true;
    }
}

void control_start(void)
{
    control_init();
}
