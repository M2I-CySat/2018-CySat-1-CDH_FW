/* 
 * File:   wire.h
 * Author: Kris
 *
 * Custom software I2C implemented while referencing code created by
 * Mike Pearces, University of Canterbury, 2000
 * http://www.microchipc.com/sourcecode/PIC_Hi-Tech_C_Mike_Pearces_I2C_routines.zip
 *
 * Created on October 29, 2012, 10:09 AM
 */

#ifndef WIRE_H
#define	WIRE_H

#include <GenericTypeDefs.h>
#include "FreeRTOS.h"

#define wireSTATUS_FAIL     0
#define wireSTATUS_SUCCESS  1
#define wireSTATUS_DEFAULT  2

#define wireBUS1    0
#define wireBUS2    1
#define wireBUS3    2
#define wireBUS4    3

typedef struct {
    /* Which I2C bus to use, wireBUS* */
    char cBus;

    /* Address ends in 0 for write and 1 for read */
    char cAddress;

    /* The data buffer to read from or write to */
    char* pcData;

    /* The number of bytes in pcData */
    char cBytes;

    /* The status of the I2C request, wireSTATUS_* */
    char* pcStatus;
} wireMessage;

void vWireStartTask();
void vWireStartTestTask();

char cWireWrite( char cAddress, char *pcData, char cBytes );
char cWireRead( char cAddress, char *pcData, char cBytes );
char cWireQueueWrite( char cBus, char cAddress, char *pcData, char cBytes );
char cWireQueueRead( char cBus, char cAddress, char *pcData, char cBytes );

#endif	/* WIRE_H */

