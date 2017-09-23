#include <stdarg.h>
#include <stdio.h>

#include "uart2.h" 
#include "mutexes.h"
#include "tasknames.h"

#include "main.h"

/* Generated file */
#include "usart.h"

int UART2_LockMutex(uint32_t max_delay)
{
	osStatus status;
	
	status =  osRecursiveMutexWait(uart2_mutexHandle, max_delay);	
	if (status != osOK) {
		return -1;
	}
	return 0;
}

int UART2_UnlockMutex()
{
	if (osRecursiveMutexRelease(uart2_mutexHandle) != osOK) {
		return -1;
	}
	return 0;
}

ssize_t UART2_Write(void *buf, size_t nbytes)
{
	if (UART2_LockMutex(osWaitForever)) {
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
	osSemaphoreWait(uart2_txSemaphoreHandle, osWaitForever);
	UART2_UnlockMutex();

	return nbytes;
}

#define FMT_BUFF_SIZE 512
static char formatBuffer[FMT_BUFF_SIZE];

ssize_t UART2_Printf(const char * fmt, ...)
{
	if (UART2_LockMutex(osWaitForever)) {
		/* TODO: Handle all error conditions */
		return -1;
	}

	va_list args;
	va_start(args, fmt);
	int len = vsnprintf(formatBuffer, FMT_BUFF_SIZE, fmt, args);

	if (len > 0) {
		UART2_Write(formatBuffer, len);
	}

	UART2_UnlockMutex();

	return len;
}

void Debug_Printf(const char * fmt, ...)
{
	if (UART2_LockMutex(osWaitForever)) {
		/* TODO: Handle all error conditions */
		return;
	}

	const char * taskname = GetTaskName();
	UART2_Printf("[ %s \t]: ", taskname);

	va_list args;
	int len = vsnprintf(formatBuffer, FMT_BUFF_SIZE, fmt, args);

	if (len > 0) {
		UART2_Write(formatBuffer, len);
	}

	UART2_Write("\r\n", 2);


	UART2_UnlockMutex();
}
