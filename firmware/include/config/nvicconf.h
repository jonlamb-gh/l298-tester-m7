/**
 * @file nvicconf.h
 * @brief TODO.
 *
 */

#ifndef NVICCONF_H
#define NVICCONF_H

// note that FreeRTOS is configured with
// configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY == 5

#define NVIC_LOW_PRI  13
#define NVIC_MID_PRI  10
#define NVIC_HIGH_PRI 7

#define NVIC_ETH_PRI        1
#define NVIC_I2C_HIGH_PRI   3
#define NVIC_I2C_LOW_PRI    4
#define NVIC_TRACE_TIM_PRI  4
#define NVIC_UART1_PRI      7
#define NVIC_UART2_PRI      7

#define NVIC_RADIO_PRI      11
#define NVIC_ADC_PRI        12
#define NVIC_CPPM_PRI       14
#define NVIC_SYSLINK_PRI    5

#define EXTI0_PRI NVIC_LOW_PRI
#define EXTI1_PRI NVIC_LOW_PRI
#define EXTI2_PRI NVIC_LOW_PRI
#define EXTI3_PRI NVIC_LOW_PRI
#define EXTI4_PRI NVIC_SYSLINK_PRI
#define EXTI9_5_PRI NVIC_LOW_PRI
#define EXTI15_10_PRI NVIC_MID_PRI

#endif /* NVICCONF_H */
