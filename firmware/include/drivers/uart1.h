/**
 * @file uart1.h
 * @brief TODO.
 *
 */

#ifndef UART1_H
#define UART1_H

#include <stdint.h>
#include <stdbool.h>

#define UART1_BAUDRATE (115200)
#define UART1_RX_TIMEOUT_MS (50)
#define UART1_RX_TIMEOUT_TICKS (UART1_RX_TIMEOUT_MS / portTICK_RATE_MS)
#define UART1_RX_QUEUE_SIZE (64)

#define UART1_TYPE USART3
#define UART1_IRQ USART3_IRQn
#define UART1_IRQ_HANDLER USART3_IRQHandler

#define UART1_CLK_ENABLE() LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3)
#define UART1_CLK_SOURCE() LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK1)
#define UART1_GPIO_CLK_ENABLE() LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD)

#define UART1_GPIO_PORT GPIOD
#define UART1_GPIO_RX_PIN LL_GPIO_PIN_9
#define UART1_GPIO_TX_PIN LL_GPIO_PIN_8
#define UART1_GPIO_AF_RX LL_GPIO_AF_7
#define UART1_GPIO_AF_TX LL_GPIO_AF_7

void uart1_init(
        const uint32_t baudrate);

bool uart1_is_init(void);

bool uart1_get_char(
        uint8_t * const data);

bool uart1_get_char_timeout(
        uint8_t * const data);

void uart1_put_char(
        const uint8_t data);

void uart1_send(
        const uint8_t * const data,
        const uint32_t size);

#endif /* UART1_H */
