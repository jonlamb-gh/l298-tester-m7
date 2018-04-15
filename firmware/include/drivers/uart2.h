/**
 * @file uart2.h
 * @brief TODO.
 *
 */

#ifndef UART2_H
#define UART2_H

#include <stdint.h>
#include <stdbool.h>

#define UART2_BAUDRATE (115200)
#define UART2_RX_TIMEOUT_MS (50)
#define UART2_RX_TIMEOUT_TICKS (UART2_RX_TIMEOUT_MS / portTICK_RATE_MS)
#define UART2_RX_QUEUE_SIZE (64)

#define UART2_TYPE USART2
#define UART2_IRQ USART2_IRQn
#define UART2_IRQ_HANDLER USART2_IRQHandler

#define UART2_CLK_ENABLE() LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2)
#define UART2_CLK_SOURCE() LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1)
#define UART2_GPIO_CLK_ENABLE() LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD)

#define UART2_GPIO_PORT GPIOD
#define UART2_GPIO_RX_PIN LL_GPIO_PIN_6
#define UART2_GPIO_TX_PIN LL_GPIO_PIN_5
#define UART2_GPIO_AF_RX LL_GPIO_AF_7
#define UART2_GPIO_AF_TX LL_GPIO_AF_7

void uart2_init(
        const uint32_t baudrate);

bool uart2_is_init(void);

bool uart2_get_char(
        uint8_t * const data);

bool uart2_get_char_timeout(
        uint8_t * const data);

void uart2_put_char(
        const uint8_t data);

void uart2_send(
        const uint8_t * const data,
        const uint32_t size);

#endif /* UART2_H */
