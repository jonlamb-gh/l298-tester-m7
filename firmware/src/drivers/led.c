/**
 * @file led.c
 * @brief TODO.
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include "stm32f7xx.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_gpio.h"
#include "debugio.h"
#include "led.h"

static bool is_init = false;

static const uint32_t LED_PINS[] =
{
    [LED_GREEN] = LED_PIN_GREEN,
    [LED_BLUE] = LED_PIN_BLUE,
    [LED_RED] = LED_PIN_RED
};

void led_init(void)
{
    if(is_init == false)
    {
        LL_GPIO_InitTypeDef gpio_init;

        LED_GPIO_CLK_ENABLE();

        LL_GPIO_StructInit(&gpio_init);
        gpio_init.Pin = (LED_PIN_GREEN | LED_PIN_BLUE | LED_PIN_RED);
        gpio_init.Mode = LL_GPIO_MODE_OUTPUT;
        gpio_init.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
        gpio_init.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
        gpio_init.Pull = LL_GPIO_PULL_NO;
        gpio_init.Alternate = 0;
        LL_GPIO_Init(LED_GPIO_PORT, &gpio_init);

        is_init = true;

        led_set_all(false);
    }
}

bool led_is_init(void)
{
    return is_init;
}

void led_set(
        const led_kind led,
        const bool state)
{
    if(state == true)
    {
        LL_GPIO_SetOutputPin(LED_GPIO_PORT, LED_PINS[led]);
    }
    else
    {
        LL_GPIO_ResetOutputPin(LED_GPIO_PORT, LED_PINS[led]);
    }
}

void led_on(
        const led_kind led)
{
    LL_GPIO_SetOutputPin(LED_GPIO_PORT, LED_PINS[led]);
}

void led_off(
        const led_kind led)
{
    LL_GPIO_ResetOutputPin(LED_GPIO_PORT, LED_PINS[led]);
}

void led_toggle(
        const led_kind led)
{
    LL_GPIO_TogglePin(LED_GPIO_PORT, LED_PINS[led]);
}

void led_set_all(
        const bool state)
{
    uint32_t i;
    for(i = 0; i < LED_COUNT; i += 1)
    {
        led_set((led_kind) i, state);
    }
}
