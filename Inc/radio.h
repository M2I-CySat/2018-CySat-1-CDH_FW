#ifndef _radio_h
#define _radio_h

#include <stdint.h>
#include <stdlib.h>

int RADIO_Transmit(uint8_t * data, size_t len);
uint8_t * RADIO_Receive(void);

#endif
