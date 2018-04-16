/**
 * @file led.h
 * @brief TODO.
 *
 */

#ifndef LED_H
#define LED_H

#include <stdbool.h>

#define LED_GPIO_PORT GPIOB

#define LED_GPIO_CLK_ENABLE() LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB)

#define LED_PIN_GREEN LL_GPIO_PIN_0
#define LED_PIN_BLUE LL_GPIO_PIN_7
#define LED_PIN_RED LL_GPIO_PIN_14

#define LED_SYSTEM_STATUS LED_RED
#define LED_COUNT (3)

typedef enum
{
    LED_GREEN = 0,
    LED_BLUE,
    LED_RED
} led_kind;

void led_init(void);

bool led_is_init(void);

void led_set(
        const led_kind led,
        const bool state);

void led_on(
        const led_kind led);

void led_off(
        const led_kind led);

void led_toggle(
        const led_kind led);

void led_set_all(
        const bool state);

#endif /* LED_H */
