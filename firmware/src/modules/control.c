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
#include "platform.h"
#include "led.h"
#include "system.h"
#include "control.h"

// PB0, green LED, and TIMER_D_PWM1 / TIM3_CH3
#define TIM3_GPIO_CHANNEL3_PORT GPIOB
#define GPIO_PIN_CHANNEL3 GPIO_PIN_0
#define GPIO_AF_TIM3 GPIO_AF2_TIM3

static StaticTask_t control_task_tcb;
static StackType_t control_task_stack[CONTROL_TASK_STACKSIZE];

static bool is_init = false;

static TIM_HandleTypeDef timer_handle;
static TIM_OC_InitTypeDef timer_oc_config;

// don't really need these, stored in ^^^
//static uint32_t timer_period;
//static uint32_t timer_duty_ccr;

static void init_gpio(void)
{
    GPIO_InitTypeDef gpio_init;

    __HAL_RCC_TIM3_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();

    gpio_init.Pin = GPIO_PIN_CHANNEL3;
    gpio_init.Mode = GPIO_MODE_AF_PP;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init.Alternate = GPIO_AF_TIM3;

    HAL_GPIO_Init(TIM3_GPIO_CHANNEL3_PORT, &gpio_init);
}

static void init_timer(
        const uint32_t period)
{
    timer_handle.Instance = TIM3;
    timer_handle.Init.Period = period;
    timer_handle.Init.RepetitionCounter = 0;
    timer_handle.Init.Prescaler = 0;
    timer_handle.Init.ClockDivision = 0;
    timer_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    timer_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if(HAL_TIM_PWM_Init(&timer_handle) != HAL_OK)
    {
        platform_error_handler();
    }
}

static void init_pwm(
        const uint32_t duty_ccr)
{
    timer_oc_config.OCMode = TIM_OCMODE_PWM1;
    timer_oc_config.OCPolarity = TIM_OCPOLARITY_HIGH;
    timer_oc_config.Pulse = duty_ccr;
    timer_oc_config.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    timer_oc_config.OCFastMode = TIM_OCFAST_DISABLE;
    timer_oc_config.OCIdleState = TIM_OCIDLESTATE_RESET;
    timer_oc_config.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    if(HAL_TIM_PWM_ConfigChannel(&timer_handle, &timer_oc_config, TIM_CHANNEL_3) != HAL_OK)
    {
        platform_error_handler();
    }
}

// Hz, 0:100
static void pwm_enable(
        const uint32_t freq,
        const uint32_t duty)
{
    debug_puts("pwm_enable");
    debug_printf("  freq %lu\r\n", freq);
    debug_printf("  duty %lu\r\n", duty);

    const uint32_t timer_period = (uint32_t) (((SystemCoreClock/2) / freq) - 1);
    const uint32_t timer_duty_ccr = (uint32_t) ((duty * (timer_period - 1)) / 100);

    debug_printf("  timer_period %lu\r\n", timer_period);
    debug_printf("  timer_duty_ccr %lu\r\n", timer_duty_ccr);

    init_timer(timer_period);

    init_pwm(timer_duty_ccr);

    if(HAL_TIM_PWM_Start(&timer_handle, TIM_CHANNEL_3) != HAL_OK)
    {
        platform_error_handler();
    }
}

static void pwm_disable(void)
{
    if(HAL_TIM_PWM_Stop(&timer_handle, TIM_CHANNEL_3) != HAL_OK)
    {
        platform_error_handler();
    }
}

static void control_task(
        void * const params)
{
    (void) params;

    system_wait_for_start();

    led_off(LED_BLUE);
    led_off(LED_GREEN);

    debug_puts(CONTROL_TASK_NAME" started");

    init_gpio();

    //init_timer();

    //init_pwm();

    // 17.57 kHz frequency
    //timer_period = (uint32_t) (((SystemCoreClock/2) / 17570) - 1);
    // 50% duty cycle
    //timer_duty_ccr = (uint32_t) (((uint32_t) 50 * (timer_period - 1)) / 100);
    // TODO - figure out the use case, make this better, what are the limits on freq?
    //pwm_enable(17570, 20);
    uint8_t pwm_state = 0;

    TickType_t last_wake_time = xTaskGetTickCount();

    while(1)
    {
        vTaskDelayUntil(&last_wake_time, M2T(500));

        led_toggle(LED_BLUE);

        if(pwm_state == 0)
        {
            pwm_enable(17570, 20);
        }
        else
        {
            pwm_disable();
        }

        pwm_state = !pwm_state;
    }

    // should not get here
    pwm_disable();
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
