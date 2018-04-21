/**
 * @file config.h
 * @brief TODO.
 *
 */

#ifndef CONFIG_H
#define CONFIG_H

// task priorities, higher number == higher priority
#define SYSTEM_TASK_PRI (2)
#define CONTROL_TASK_PRI (5)
#define CONTROL_IO_INPUT_TASK_PRI (4)

// task stack sizes
#define IDLE_TASK_STACKSIZE (3 * configMINIMAL_STACK_SIZE)
#define SYSTEM_TASK_STACKSIZE (2 * configMINIMAL_STACK_SIZE)
#define CONTROL_TASK_STACKSIZE (5 * configMINIMAL_STACK_SIZE)
#define CONTROL_IO_INPUT_TASK_STACKSIZE (5 * configMINIMAL_STACK_SIZE)

// task names
#define SYSTEM_TASK_NAME "SYS"
#define CONTROL_TASK_NAME "CONTROL"
#define CONTROL_IO_INPUT_TASK_NAME "CNTRL_IO_INPUT"

// 1 for FreeRTOS internal idle task
// 1 for LwIP internals (tcp/ip) (NOT_USED)
#define TASK_COUNT (4)

#define IP_ADDR0 (192)
#define IP_ADDR1 (168)
#define IP_ADDR2 (1)
#define IP_ADDR3 (39)

#define NETMASK_ADDR0 (255)
#define NETMASK_ADDR1 (255)
#define NETMASK_ADDR2 (255)
#define NETMASK_ADDR3 (0)

#define GW_ADDR0 (192)
#define GW_ADDR1 (168)
#define GW_ADDR2 (1)
#define GW_ADDR3 (1)

#define UDPSERVER_PORT (57319)

#endif /* CONFIG_H */
