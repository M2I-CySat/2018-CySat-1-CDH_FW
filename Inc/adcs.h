#ifndef _adcs_h
#define _adcs_h

#include <stdint.h>

#define ADCS_WRITE_ADDRESS 0xAE
#define ADCS_READ_ADDRESS 0xAF

int ADCS_Get_Data(uint8_t data);
int ADCS_Send_Command(uint8_t command);

int ADCS_Init(void);

#endif
