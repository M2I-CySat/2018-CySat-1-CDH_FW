#include "mem.h"
#include "cmsis_os.h"
#include "mutexes.h"

#include "spi.h"
#include "gpio.h"

#include "uart2.h"

#define OP_READ 0x03

int MEM_LockMutex(uint32_t max_delay)
{
	osStatus status;
	status = osRecursiveMutexWait(mem_mutexHandle, max_delay);
	if (status != osOK) {
		return -1;
	}
	return 0;
}

int MEM_UnlockMutex()
{
	if (osRecursiveMutexRelease(mem_mutexHandle) != osOK) {
		return -1;	
	}
	return 0;
}

ssize_t MEM_Read(uint8_t * buf, size_t addr, size_t len)
{

	int retval;
	MEM_LockMutex(osWaitForever);

	size_t chip = (addr >> 18);

	if ((addr + len) >> 18 != chip) {
		Debug_Printf("Read would cause chip boundary crossover.");
		goto releaseMutex;
	}

	GPIO_TypeDef * port;
	uint16_t pin;

	switch (chip) {
		case 0:
			port = MEM_CS0_GPIO_Port;
			pin = MEM_CS0_Pin;
			break;
		case 1:
			port = MEM_CS1_GPIO_Port;
			pin = MEM_CS1_Pin;
			break;
		case 2:
			port = MEM_CS2_GPIO_Port;
			pin = MEM_CS2_Pin;
			break;
		case 3:
			port = MEM_CS3_GPIO_Port;
			pin = MEM_CS3_Pin;
			break;
		default:
			retval = -1;
			goto releaseMutex;
	}

	/* Begin transaction */
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);

	uint8_t op[4];
	op[0] = OP_READ;
	op[1] = (addr & 0x030000) >> 16;
	op[2] = (addr & 0x00ff00) >> 8;
	op[3] = (addr & 0x0000ff);

	/* Send opcode and address */
	HAL_StatusTypeDef status;
	status = HAL_SPI_Transmit_DMA(&hspi2, op, 4);
	if (status) {
		retval = -1;
		goto endTransaction;
	}
	osSemaphoreWait(mem_semaphoreHandle, osWaitForever);

	/* Read data */
	status = HAL_SPI_Receive_DMA(
			&hspi2,
			buf,
			len);
	if (status) {
		retval = -1;
		goto endTransaction;
	}
	osSemaphoreWait(mem_semaphoreHandle, osWaitForever);

	retval = len;

endTransaction:
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
releaseMutex:
	MEM_UnlockMutex();
	return retval;
}
