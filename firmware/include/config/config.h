/**
 * @file config.h
 * @brief TODO.
 *
 */

#ifndef CONFIG_H
#define CONFIG_H

// task priorities, higher number == higher priority
#define SYSTEM_TASK_PRI (2)
#define ETHIF_TASK_PRI (6)
#define UDPSERVER_INIT_TASK_PRI (2)
#define UDPSERVER_DATA_TASK_PRI (3)
#define UDPSERVER_IO_TASK_PRI (4)
#define IMU_RX_TASK_PRI (3)

// task stack sizes
#define IDLE_TASK_STACKSIZE (3 * configMINIMAL_STACK_SIZE)
#define SYSTEM_TASK_STACKSIZE (2 * configMINIMAL_STACK_SIZE)
#define ETHIF_TASK_STACKSIZE (3 * configMINIMAL_STACK_SIZE)
#define UDPSERVER_INIT_TASK_STACKSIZE (5 * configMINIMAL_STACK_SIZE)
#define UDPSERVER_DATA_TASK_STACKSIZE (3 * configMINIMAL_STACK_SIZE)
#define UDPSERVER_IO_TASK_STACKSIZE (2 * configMINIMAL_STACK_SIZE)
#define IMU_RX_TASK_STACKSIZE (3 * configMINIMAL_STACK_SIZE)

// task names
#define SYSTEM_TASK_NAME "SYS"
#define ETHIF_TASK_NAME "ETHIF"
#define UDPSERVER_INIT_TASK_NAME "UDPS-INIT"
#define UDPSERVER_DATA_TASK_NAME "UDPS-DATA"
#define UDPSERVER_IO_TASK_NAME "UDPS-IO"
#define IMU_RX_TASK_NAME "IMU-RX"

// 1 for FreeRTOS internal idle task
// 1 for LwIP internals (tcp/ip)
#define TASK_COUNT (8)

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
