#ifndef _mem_h
#define _mem_h

#include "main.h"
#include <stdint.h>
#include <sys/types.h>

int MEM_LockMutex(uint32_t max_delay);
int MEM_UnlockMutex();

ssize_t MEM_Read(uint8_t * buf, size_t addr, size_t len);
ssize_t MEM_Write(uint8_t * buf, size_t addr, size_t len);

/* Status register operations.
 * Addr: Relevant address, used to determine chip select */
int MEM_WriteEnable(size_t addr);
int MEM_WriteDisable(size_t addr);
int MEM_ReadStatus(size_t addr);
int MEM_WriteStatus(size_t addr, uint8_t status);

#endif
