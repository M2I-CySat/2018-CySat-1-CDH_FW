/* This file implements all of the relevant callbacks */

#include "mutexes.h"
#include "usart.h"
#include "spi.h"
#include "stm32f4xx_hal_uart.h"
#include "cmsis_os.h"

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart6) {
		osSemaphoreRelease(uart2_txSemaphoreHandle);
	}
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi == &hspi2) {
		osSemaphoreRelease(mem_semaphoreHandle);
	}
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi == &hspi2) {
		osSemaphoreRelease(mem_semaphoreHandle);
	}
}
