#ifndef _mem_h
#define _mem_h

#include "main.h"
#include <stdint.h>
#include <sys/types.h>

void MEM_LockMutex(void);
void MEM_UnlockMutex(void);

ssize_t MEM_Read(uint8_t * buf, size_t addr, size_t len);
ssize_t MEM_Write(uint8_t * buf, size_t addr, size_t len);

/* Status register operations.
 * Addr: Relevant address, used to determine chip select */
int MEM_WriteEnable(size_t addr);
int MEM_WriteDisable(size_t addr);
int MEM_ReadStatus(size_t addr);
int MEM_WriteStatus(size_t addr, uint8_t status);

/* Memory map defines */

#define TEST_FLAG_ADDRESS 		0x00000010
#define TEST_FLAG_LENGTH 		0x04

#define REBOOT_COUNT_ADDRESS 	0x00000014
#define REBOOT_COUNT_LENGTH 	0x04
//the amount of bytes that each item takes up
#define HEAP_ITEM_SIZE			0x00000040
//the most amount of items we can have in the heap at a time
#define HEAP_NUM_ITEMS			0x00001000

#define HEAP_START 				0x00001000
//the amount of bytes that the heap takes up
#define HEAP_SIZE ((HEAP_ITEM_SIZE * HEAP_NUM_ITEMS))
#define HEAP_END ((HEAP_START + HEAP_SIZE))
//the memory address where the variable for the current number of elements is stored
#define HEAP_CURRENT_SIZE ((HEAP_END))
#endif
