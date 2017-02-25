#ifndef _mem_h
#define _mem_h

#include "main.h"
#include <stdint.h>
#include <sys/types.h>

int MEM_LockMutex(uint32_t max_delay);
int MEM_UnlockMutex();

ssize_t MEM_Read(uint8_t * buf, size_t addr, size_t len);
ssize_t MEM_Write(uint8_t * buf, size_t addr, size_t len);

#endif
