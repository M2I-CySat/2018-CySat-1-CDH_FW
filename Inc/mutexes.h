#ifndef _mutexes_h
#define _mutexes_h
/* Extern declarations for all mutex handles */

#include "cmsis_os.h"
#include "FreeRTOS.h"

/* Mutexes */
extern osMutexId uart2_mutexHandle;
extern osMutexId mem_mutexHandle;
extern osMutexId sys_i2c_mutexHandle;
extern osMutexId threadnames_mutexHandle;
extern osMutexId rtc_mutexHandle;
extern osMutexId imu_mutexHandle;
extern osMutexId eps_mutexHandle;

/* Semaphores */
extern osSemaphoreId uart2_txSemaphoreHandle;
extern osSemaphoreId radio_txSemaphoreHandle;
extern osSemaphoreId radio_rxSemaphoreHandle;
extern osSemaphoreId mem_semaphoreHandle;
extern osSemaphoreId sys_i2c_semaphoreHandle;
extern osSemaphoreId eps_semaphoreHandle;


#endif
