#ifndef _uart2_h
#define _uart2_h

/* Defines safe wrapper functions for using UART2 */

#include <stdint.h>
#include <sys/types.h>

#include "FreeRTOS.h"

/* Lock the UART2 mutex. Returns 0 on success, nonzero on timeout */
int UART2_LockMutex(uint32_t max_delay);

/* Unlock the UART2 mutex. Returns 0 on success */
int UART2_UnlockMutex();

/* Read, write, printf */
ssize_t UART2_Read(void * buf, size_t nbytes);
ssize_t UART2_Write(void * buf, size_t nbytes);
ssize_t UART2_Printf(const char * fmt, ...);

#endif
