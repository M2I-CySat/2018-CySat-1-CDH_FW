/* 
 * File:   system.h
 * Author: Kris
 *
 * Created on October 30, 2012, 8:49 AM
 */

#ifndef SYSTEM_H
#define	SYSTEM_H

#include "task.h"

#define systemPRIORITY_BASE     ( tskIDLE_PRIORITY + 3 )


#define systemPRIORITY_UART1    ( systemPRIORITY_BASE + 05 )

#define systemPRIORITY_UART2    ( systemPRIORITY_BASE + 10 )

#define systemPRIORITY_WIRE     ( systemPRIORITY_BASE + 15 )

#define systemPRIORITY_POWER    ( systemPRIORITY_BASE + 20 )

#define systemPRIORITY_COMMS    ( systemPRIORITY_BASE + 25 )

#define systemPRIORITY_PAYLOAD  ( systemPRIORITY_BASE + 30 )

#define systemPRIORITY_CLOCK    ( systemPRIORITY_BASE + 35 )

#define systemPRIORITY_INIT     ( systemPRIORITY_BASE + 40 )

#define systemPRIORITY_STORAGE  ( systemPRIORITY_BASE + 45 )

#define systemPRIORITY_NICHROME ( systemPRIORITY_BASE + 50 )

#define systemPRIORITY_TEST     ( systemPRIORITY_BASE + 55 )


#endif	/* SYSTEM_H */

