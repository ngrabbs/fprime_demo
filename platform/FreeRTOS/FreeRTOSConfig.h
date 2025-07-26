/*
 * FreeRTOSConfig.h
 *
 * This configuration header is provided as a starting point for
 * integrating FreeRTOS with F´ on the RP2040.  The settings here are
 * intentionally minimal and may need to be tuned for your specific
 * application.  Refer to the FreeRTOS documentation for a full
 * description of each option.
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdint.h>

/* Hardware specifics for the RP2040 (dual‑core Cortex‑M0+).  F´ runs on a
 * single core; modify configNUMBER_OF_CORES if using SMP. */
#define configCPU_CLOCK_HZ            ( 125000000UL )
#define configTICK_RATE_HZ           ( 1000 )
#define configMINIMAL_STACK_SIZE     ( 256 )
#define configTOTAL_HEAP_SIZE        ( 64 * 1024 )

/* Task priorities */
#define configMAX_PRIORITIES         ( 5 )

/* Mutexes and semaphores */
#define configUSE_MUTEXES            1
#define configUSE_COUNTING_SEMAPHORES 1

/* Co-routines are not used */
#define configUSE_CO_ROUTINES        0

/* Hook function stubs – disabled for now */
#define configUSE_IDLE_HOOK          0
#define configUSE_TICK_HOOK          0
#define configCHECK_FOR_STACK_OVERFLOW 2

/* Run time stats gathering functions (optional) */
#define configGENERATE_RUN_TIME_STATS 0

/* API function inclusion */
#define INCLUDE_vTaskPrioritySet     1
#define INCLUDE_uxTaskPriorityGet    1
#define INCLUDE_vTaskDelete          1
#define INCLUDE_vTaskSuspend         1
#define INCLUDE_vTaskDelay           1
#define INCLUDE_xTaskGetSchedulerState 1

/* For static allocation support (optional) */
#define configSUPPORT_STATIC_ALLOCATION 0

/* Interrupt priority configuration */
#define configKERNEL_INTERRUPT_PRIORITY      ( 0xFF )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY ( 0x10 )

#endif /* FREERTOS_CONFIG_H */