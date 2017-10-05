#ifndef _utilities_h
#define _utilities_h

#include <stdint.h>

uint32_t Unpack32(uint8_t * buf);
void Pack32(uint8_t * buf, uint32_t flag);

#endif
