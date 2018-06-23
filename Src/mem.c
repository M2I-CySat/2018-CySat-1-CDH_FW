#include "mem.h"
#include "cmsis_os.h"
#include "mutexes.h"

#include "spi.h"
#include "gpio.h"

#include "uart2.h"

#include "assert.h"

#define OP_READ 0x03
#define OP_WRITE 0x02
#define OP_RDSR 0x05
#define OP_WRSR 0x01
#define OP_WREN 0x06
#define OP_WRDI 0x04
#define MEM_MAX_DELAY osWaitForever

static int getChipSelect(size_t addr, GPIO_TypeDef ** port, uint16_t * pin);
static int simpleSend(size_t addr, uint8_t * data, size_t len);

void MEM_LockMutex(void)
{
	if (osRecursiveMutexWait(mem_mutexHandle, MEM_MAX_DELAY) != osOK) {
		hard_failure();
	}
}

void MEM_UnlockMutex(void)
{
	if (osRecursiveMutexRelease(mem_mutexHandle) != osOK) {
		hard_failure();
	}
}

int MEM_ReadStatus(size_t addr)
{
	int retval;
	MEM_LockMutex();

	GPIO_TypeDef * port;
	uint16_t pin;
	int chip = getChipSelect(addr, &port, &pin);

	if (chip < 0) {
		retval = -1;
		goto releaseMutex;
	}

	/* Begin transaction */
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);

	uint8_t op = OP_RDSR;
	HAL_StatusTypeDef status;
	status = HAL_SPI_Transmit_DMA(&hspi2, &op, 1);
	if (status) {
		retval = -1;
		goto endTransaction;
	}
	osSemaphoreWait(mem_semaphoreHandle, osWaitForever);

	/* Read data */
	status = HAL_SPI_Receive_IT(
			&hspi2,
			&op,
			1);
	if (status) {
		retval = -1;
		goto endTransaction;
	}
	osSemaphoreWait(mem_semaphoreHandle, osWaitForever);

	retval = op;

endTransaction:
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
releaseMutex:
	MEM_UnlockMutex();
	return retval;
}

ssize_t MEM_Read(uint8_t * buf, size_t addr, size_t len)
{

	int retval;
	MEM_LockMutex();

	GPIO_TypeDef * port;
	uint16_t pin;
	int chip = getChipSelect(addr, &port, &pin);

	if (chip < 0) {
		retval = -1;
		goto releaseMutex;
	}

	/* TODO: Fix this logic! */
	if ((addr + len) >> 18 != ((unsigned int) chip)) {
		Debug_Printf("Read would cause chip boundary crossover.");
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
	status = HAL_SPI_Receive_IT(
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

ssize_t MEM_Write(uint8_t * buf, size_t addr, size_t len)
{

	int retval;
	MEM_LockMutex();

	GPIO_TypeDef * port;
	uint16_t pin;
	int chip = getChipSelect(addr, &port, &pin);

	if (chip < 0) {
		retval = -1;
		goto releaseMutex;
	}

	/* TODO: Fix this logic */
	if ((addr + len) >> 18 != ((unsigned int) chip)) {
		Debug_Printf("Read would cause chip boundary crossover.");
		retval = -1;
		goto releaseMutex;
	}

	/* Begin transaction */
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);

	uint8_t op[4];
	op[0] = OP_WRITE;
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
	status = HAL_SPI_Transmit_DMA(
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

int MEM_WriteEnable(size_t addr)
{
	uint8_t data = OP_WREN;
	return simpleSend(addr, &data, 1);
}

int MEM_WriteDisable(size_t addr)
{
	uint8_t data = OP_WRDI;
	return simpleSend(addr, &data, 1);
}

int MEM_WriteStatus(size_t addr, uint8_t st)
{
	uint8_t data[2];
	data[0] = OP_WRSR;
	data[1] = st;
	return simpleSend(addr, data, 2);
}

static int getChipSelect(size_t addr, GPIO_TypeDef ** port, uint16_t * pin)
{
	int chip = (addr >> 18);


	switch (chip) {
		case 0:
			*port = MEM_CS0_GPIO_Port;
			*pin = MEM_CS0_Pin;
			break;
		case 1:
			*port = MEM_CS1_GPIO_Port;
			*pin = MEM_CS1_Pin;
			break;
		case 2:
			*port = MEM_CS2_GPIO_Port;
			*pin = MEM_CS2_Pin;
			break;
		case 3:
			*port = MEM_CS3_GPIO_Port;
			*pin = MEM_CS3_Pin;
			break;
		default:
			Debug_Printf("Invalid address.");
			return -1;
	}

	*port = MEM_CS1_GPIO_Port;
	*pin = MEM_CS1_Pin;
	return chip;
}

static int simpleSend(size_t addr, uint8_t * data, size_t len)
{
	int retval;
	MEM_LockMutex();

	GPIO_TypeDef * port;
	uint16_t pin;
	int chip = getChipSelect(addr, &port, &pin);

	if (chip < 0) {
		retval = -1;
		goto releaseMutex;
	}

	if ((addr + len) >> 18 != ((unsigned int) chip)) {
		Debug_Printf("Read would cause chip boundary crossover.");
		retval = -1;
		goto releaseMutex;
	}

	/* Begin transaction */
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);

	HAL_StatusTypeDef status;
	status = HAL_SPI_Transmit_DMA(&hspi2, data, len);
	if (status) {
		retval = -1;
		goto endTransaction;
	}
	osSemaphoreWait(mem_semaphoreHandle, osWaitForever);

	retval = 1;

endTransaction:
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
releaseMutex:
	MEM_UnlockMutex();
	return retval;
}
