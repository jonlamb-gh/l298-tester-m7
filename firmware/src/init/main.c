/**
 * @file main.c
 * @brief Main.
 *
 */

#include "FreeRTOS.h"
#include "task.h"
#include "platform.h"
#include "system.h"

int main(void)
{
    platform_init();

    system_start();

    vTaskStartScheduler();

    // should never get here
    while(1)
    {
        platform_error_handler();
    }

    return 0;
}
