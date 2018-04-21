/**
 * @file control_io.c
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
#include "system.h"
#include "control_io.h"

// PC0/ADC123_IN10 on CN9 pin 3
#define PT0_ADC_CHANNEL_PIN GPIO_PIN_0
#define PT0_ADC_CHANNEL_GPIO_PORT GPIOC
#define PT0_ADC_CHANNEL ADC_CHANNEL_10

// PC3/ADC123_IN13 on CN9 pin 5
#define PT1_ADC_CHANNEL_PIN GPIO_PIN_3
#define PT1_ADC_CHANNEL_GPIO_PORT GPIOC
#define PT1_ADC_CHANNEL ADC_CHANNEL_13

#define ADC_SAMPLE_TIME ADC_SAMPLETIME_3CYCLES

#define ADC_DMA_CHANNEL DMA_CHANNEL_0
#define ADC_DMA_STREAM DMA2_Stream0

#define ADC_DMA_IRQ DMA2_Stream0_IRQn
#define ADC_DMA_IRQHandler DMA2_Stream0_IRQHandler

static StaticTask_t input_task_tcb;
static StackType_t input_task_stack[CONTROL_IO_INPUT_TASK_STACKSIZE];

static ADC_HandleTypeDef adc_handle;
static DMA_HandleTypeDef dma_handle;

static volatile uint32_t adc_samples[2];

static bool is_init = false;

void ADC_DMA_IRQHandler(void)
{
    HAL_DMA_IRQHandler(adc_handle.DMA_Handle);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *handle)
{
    (void) handle;
    // TODO
}

static void init_gpio(void)
{
    GPIO_InitTypeDef gpio_init;

    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();

    gpio_init.Pin = PT0_ADC_CHANNEL_PIN;
    gpio_init.Mode = GPIO_MODE_ANALOG;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init.Alternate = 0;

    HAL_GPIO_Init(PT0_ADC_CHANNEL_GPIO_PORT, &gpio_init);

    gpio_init.Pin = PT0_ADC_CHANNEL_PIN;

    HAL_GPIO_Init(PT1_ADC_CHANNEL_GPIO_PORT, &gpio_init);
}

static void init_dma(void)
{
    __HAL_RCC_DMA2_CLK_ENABLE();

    dma_handle.Instance = ADC_DMA_STREAM;

    dma_handle.Init.Channel  = ADC_DMA_CHANNEL;
    dma_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;
    dma_handle.Init.MemInc = DMA_MINC_ENABLE;
    dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    dma_handle.Init.Mode = DMA_CIRCULAR;
    dma_handle.Init.Priority = DMA_PRIORITY_HIGH;
    dma_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    dma_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    dma_handle.Init.MemBurst = DMA_MBURST_SINGLE;
    dma_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;

    HAL_DMA_Init(&dma_handle);

    __HAL_LINKDMA(&adc_handle, DMA_Handle, dma_handle);

    HAL_NVIC_SetPriority(ADC_DMA_IRQ, 0, 0);
    HAL_NVIC_EnableIRQ(ADC_DMA_IRQ);
}

static void init_adc(void)
{
    ADC_ChannelConfTypeDef adc_config;

    adc_handle.Instance = ADC1;

    if(HAL_ADC_DeInit(&adc_handle) != HAL_OK)
    {
        platform_error_handler();
    }

    __HAL_RCC_ADC_FORCE_RESET();
    __HAL_RCC_ADC_RELEASE_RESET();

    adc_handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
    adc_handle.Init.Resolution = ADC_RESOLUTION_12B;
    adc_handle.Init.ScanConvMode = ADC_SCAN_ENABLE;
    adc_handle.Init.ContinuousConvMode = ENABLE;
    adc_handle.Init.DiscontinuousConvMode = DISABLE;
    adc_handle.Init.NbrOfDiscConversion = 0;
    adc_handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    adc_handle.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
    adc_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    adc_handle.Init.NbrOfConversion = 2;
    adc_handle.Init.DMAContinuousRequests = ENABLE;
    adc_handle.Init.EOCSelection = DISABLE;

    adc_config.Channel = PT0_ADC_CHANNEL;
    adc_config.Rank = ADC_REGULAR_RANK_1;
    adc_config.SamplingTime = ADC_SAMPLE_TIME;
    adc_config.Offset = 0;

    if(HAL_ADC_ConfigChannel(&adc_handle, &adc_config) != HAL_OK)
    {
        platform_error_handler();
    }

    adc_config.Channel = PT1_ADC_CHANNEL;
    adc_config.Rank = ADC_REGULAR_RANK_2;
    adc_config.SamplingTime = ADC_SAMPLE_TIME;
    adc_config.Offset = 0;

    if(HAL_ADC_ConfigChannel(&adc_handle, &adc_config) != HAL_OK)
    {   
        platform_error_handler();
    }
}

static void enable_adc(void)
{
    if(HAL_ADC_Start_DMA(&adc_handle, (uint32_t*) &adc_samples[0], 2) != HAL_OK)
    {
        platform_error_handler();
    }
}

static void input_task(
        void * const params)
{
    (void) params;

    system_wait_for_start();

    debug_puts(CONTROL_IO_INPUT_TASK_NAME" started");

    init_gpio();

    init_adc();

    init_dma();

    enable_adc();

    TickType_t last_wake_time = xTaskGetTickCount();

    while(1)
    {
        vTaskDelayUntil(&last_wake_time, M2T(25));

        debug_printf("pt0 %lu\r\n", adc_samples[0]);
        debug_printf("pt1 %lu\r\n", adc_samples[1]);
    }

    // should not get here
}

static void control_io_init(void)
{
    if(is_init == false)
    {
        (void) xTaskCreateStatic(
                &input_task,
                CONTROL_IO_INPUT_TASK_NAME,
                CONTROL_IO_INPUT_TASK_STACKSIZE,
                NULL,
                CONTROL_IO_INPUT_TASK_PRI,
                &input_task_stack[0],
                &input_task_tcb);

        is_init = true;
    }
}

void control_io_start(void)
{
    control_io_init();
}
