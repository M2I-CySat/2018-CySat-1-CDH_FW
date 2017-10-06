#ifndef _sys_ic2_h
#define _sys_i2c_h

#include <stdint.h>
#include <stdlib.h>

int SYS_I2C_Read(uint8_t address, uint8_t * data, size_t len);
int SYS_I2C_Write(uint8_t address, uint8_t * data, size_t len);

void SYS_I2C_LockMutex(void);
void SYS_I2C_UnlockMutex(void);

#define I2C_ADDRESS_RADIO 0x02 /* placeholder */
#define I2C_ADDRESS_EPS 0x03   /* placeholder */


#endif
