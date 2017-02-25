#ifndef _mutexes_h
#define _mutexes_h
/* Extern declarations for all mutex handles */

#include "cmsis_os.h"
#include "FreeRTOS.h"

/* Mutexes */
extern osMutexId uart2_mutexHandle;
extern osMutexId mem_mutexHandle;

/* Semaphores */
extern osSemaphoreId uart2_txSemaphoreHandle;
extern osSemaphoreId mem_semaphoreHandle;


#endif
