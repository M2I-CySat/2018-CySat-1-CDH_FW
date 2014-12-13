/* 
 * File:   system.h
 * Author: Kris
 *
 * Created on October 30, 2012, 8:49 AM
 */

#ifndef SYSTEM_H
#define	SYSTEM_H

#include "task.h"

#define systemPRIORITY_BASE     ( tskIDLE_PRIORITY + 1 )


#define systemPRIORITY_UART1    ( systemPRIORITY_BASE + 2 )

#define systemPRIORITY_UART2    ( systemPRIORITY_BASE + 2 )

#define systemPRIORITY_WIRE     ( systemPRIORITY_BASE + 3 )

#define systemPRIORITY_POWER    ( systemPRIORITY_BASE + 4 )

#define systemPRIORITY_COMMS    ( systemPRIORITY_BASE + 5 )

#define systemPRIORITY_PAYLOAD  ( systemPRIORITY_BASE + 6 )

#define systemPRIORITY_CLOCK    ( systemPRIORITY_BASE + 7 )

#define systemPRIORITY_INIT     ( systemPRIORITY_BASE + 8 )

#define systemPRIORITY_STORAGE  ( systemPRIORITY_BASE + 9 )

#define systemPRIORITY_NICHROME ( systemPRIORITY_BASE + 10 )

#define systemPRIORITY_TEST     ( systemPRIORITY_BASE + 11 )


#define systemDEFAULT_STACK_SIZE ( 512 )

#define bitSet(a, b) ((a) |= (b))
#define bitClear(a, b) ((a) &= ~(b))

#define tmpGet(b) ((tmp) = (b))
#define tmpSet(b) ((tmp) |= (b))
#define tmpClear(b) ((tmp) &= ~(b))
#define tmpPut(b) ((b) = (tmp))

#endif	/* SYSTEM_H */

