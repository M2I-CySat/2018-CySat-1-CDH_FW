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

typedef struct {
    void (*callback)(void);
} wireMessage;

void vWireStartTask();
char vWireWrite( char cAddress, char *pcData, char cBytes );
char vWireRead( char cAddress, char *pcData, char cBytes );

#endif	/* WIRE_H */

