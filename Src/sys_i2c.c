#include "sys_i2c.h"
#include "cmsis_os.h"
#include "mutexes.h"
#include "assert.h"

#include "i2c.h"

#define I2C_MAX_DELAY osWaitForever

int sys_i2c_cb_status;

void SYS_I2C_LockMutex(void)
{
	osStatus status;
	status = osRecursiveMutexWait(sys_i2c_mutexHandle, I2C_MAX_DELAY);
	if (status != osOK) {
		hard_failure();
	}
}

void SYS_I2C_UnlockMutex(void)
{
	if (osRecursiveMutexRelease(sys_i2c_mutexHandle) != osOK) {
		hard_failure();
	}
}

int SYS_I2C_Write(uint8_t addr, uint8_t * data, size_t len)
{
	int retval = 0;
	SYS_I2C_LockMutex();

	int status = HAL_I2C_Master_Transmit_DMA(&hi2c1, addr, data, len);
	if (status) {
		retval = -1;
		goto releaseMutex;
	}
	osSemaphoreWait(sys_i2c_semaphoreHandle, osWaitForever);

releaseMutex:
	SYS_I2C_UnlockMutex();

	return retval;
}

int SYS_I2C_Read(uint8_t addr, uint8_t * data, size_t len)
{
	int retval = 0;
	SYS_I2C_LockMutex();

	int status = HAL_I2C_Master_Receive_DMA(&hi2c1, addr, data, len);
	if (status) {
		retval = -1;
		goto releaseMutex;
	}
	osSemaphoreWait(sys_i2c_semaphoreHandle, osWaitForever);

releaseMutex:
	SYS_I2C_UnlockMutex();

	return retval;
}
