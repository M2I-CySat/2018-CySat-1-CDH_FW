/* 
 * File:   wire.h
 * Author: Kris
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
void vWireRequest(wireMessage* xMessage);

void vWireInit();
void vWireStart();
void vWireRestart();
void vWireStop();
void vWireWait();
void vWireWrite(BYTE data);
BYTE ucWireRead();
BYTE ucWireReadAndStop();

#endif	/* WIRE_H */

