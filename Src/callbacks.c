/* This file implements all of the relevant callbacks */

#include "mutexes.h"
#include "usart.h"
#include "spi.h"
#include "i2c.h"
#include "stm32f4xx_hal_uart.h"
#include "cmsis_os.h"

extern int sys_i2c_cb_status;

static void i2c_callback(I2C_HandleTypeDef *hi2c, int status);

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

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	i2c_callback(hi2c, 0);
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	i2c_callback(hi2c, 0);
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	i2c_callback(hi2c, -1);
}

static void i2c_callback(I2C_HandleTypeDef *hi2c, int status)
{
	if (hi2c == &hi2c1) {
		sys_i2c_cb_status = status; 
		osSemaphoreRelease(sys_i2c_semaphoreHandle);
	}
}
