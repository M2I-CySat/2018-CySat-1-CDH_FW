#include "uart2.h" 
#include "mutexes.h"

#include "main.h"

/* Generated file */
#include "usart.h"

int UART2_LockMutex(uint32_t max_delay)
{
	osStatus status;
	
	status =  osMutexWait(uart2_mutexHandle, max_delay);	
	if (status != osOK) {
		/* TODO: Handle all error conditions */
		return -1;
	}
	return 0;
}

int UART2_UnlockMutex()
{
	if (osMutexRelease(uart2_mutexHandle) != osOK) {
		return -1;
	}
	return 0;
}

ssize_t UART2_Write(void *buf, size_t nbytes)
{
	if (UART2_LockMutex(0)) {
		return -1;
	}

	HAL_StatusTypeDef status = HAL_UART_Transmit_DMA(
			&huart6,
			(uint8_t *) buf,
			nbytes);
	if (status) {
		UART2_UnlockMutex();
		return -1;
	}

	/* Wait for complete semaphore, then unlock mutex and return */
	osSemaphoreWait(&uart2_txSemaphoreHandle, 0);
	UART2_UnlockMutex();

	return nbytes;
}
