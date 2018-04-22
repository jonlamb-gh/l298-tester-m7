/**
 * @file platform.c
 * @brief TODO.
 *
 */

#include <string.h>
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "debug.h"
#include "led.h"
#include "platform.h"

#ifdef BUILD_TYPE_DEBUG
#include "uart1.h"
#endif

/**
  * @brief  Configure the MPU attributes as Device for  Ethernet Descriptors in the SRAM1.
  * @note   The Base Address is 0x20020000 since this memory interface is the AXI.
  *         The Configured Region Size is 256B (size of Rx and Tx ETH descriptors)
  */
static void mpu_config(void)
{
    MPU_Region_InitTypeDef mpu_init;

    HAL_MPU_Disable();

    memset(&mpu_init, 0, sizeof(mpu_init));
    mpu_init.Enable = MPU_REGION_ENABLE;
    mpu_init.BaseAddress = 0x20020000;
    mpu_init.Size = MPU_REGION_SIZE_256B;
    mpu_init.AccessPermission = MPU_REGION_FULL_ACCESS;
    mpu_init.IsBufferable = MPU_ACCESS_BUFFERABLE;
    mpu_init.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    mpu_init.IsShareable = MPU_ACCESS_SHAREABLE;
    mpu_init.Number = MPU_REGION_NUMBER0;
    mpu_init.TypeExtField = MPU_TEX_LEVEL0;
    mpu_init.SubRegionDisable = 0x00;
    mpu_init.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
    HAL_MPU_ConfigRegion(&mpu_init);

    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

static void cpu_cache_enable(void)
{
    SCB_EnableICache();

    SCB_EnableDCache();
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 200000000
  *            HCLK(Hz)                       = 200000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 400
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            PLL_R                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  */
static void system_clock_config(void)
{
    RCC_ClkInitTypeDef clk_init;
    RCC_OscInitTypeDef osc_init;

    memset(&clk_init, 0, sizeof(clk_init));
    memset(&osc_init, 0, sizeof(osc_init));

    // enable power control clock
    __HAL_RCC_PWR_CLK_ENABLE();

    // enable voltage scaling
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    // enable HSE oscillator and activate PLL with HSE as source
    osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    osc_init.HSEState = RCC_HSE_ON;
    osc_init.HSIState = RCC_HSI_OFF;
    osc_init.PLL.PLLState = RCC_PLL_ON;
    osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    osc_init.PLL.PLLM = 25;
    osc_init.PLL.PLLN = 400;
    osc_init.PLL.PLLP = RCC_PLLP_DIV2;
    osc_init.PLL.PLLQ = 9;
    osc_init.PLL.PLLR = 7;

    if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
    {
        platform_error_handler();
    }

    // activate OverDrive
    if(HAL_PWREx_EnableOverDrive() != HAL_OK)
    {
        platform_error_handler();
    }

    // select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
    // clocks dividers
    clk_init.ClockType =
            (RCC_CLOCKTYPE_SYSCLK |
             RCC_CLOCKTYPE_HCLK |
             RCC_CLOCKTYPE_PCLK1 |
             RCC_CLOCKTYPE_PCLK2);
    clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
    clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

    if(HAL_RCC_ClockConfig(&clk_init, FLASH_LATENCY_7) != HAL_OK)
    {
        platform_error_handler();
    }
}

void platform_init(void)
{
    mpu_config();

    cpu_cache_enable();

    HAL_Init();

    system_clock_config();

    led_init();

    debug_init();
}

void platform_error_handler(void)
{
    debug_printf("!!! entering platfrom error handler !!!\r\n");

    while(1)
    {
        led_set_all(true);
    }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    platform_error_handler();
}
#endif

#ifdef BUILD_TYPE_DEBUG
int __io_putchar(int ch)
{
    uart1_put_char((uint8_t) ch);

    return ch;
}

int _write(int file, char *ptr, int len)
{
    (void) file;
    int idx;

    for(idx = 0; idx < len; idx += 1)
    {
        __io_putchar(*ptr++);
    }

    return len;
}

void vApplicationStackOverflowHook(
        TaskHandle_t task,
        signed char *task_name)
{
    (void) task;

    debug_printf("ERROR stack overflow detected on task '%s'\r\n", task_name);

    platform_error_handler();
}
#endif
