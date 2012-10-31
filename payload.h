/* 
 * File:   payload.h
 * Author: Kris
 *
 * Created on October 30, 2012, 2:51 PM
 */

#ifndef PAYLOAD_H
#define	PAYLOAD_H

#include <GenericTypeDefs.h>

typedef struct {
    UINT ulData0;
    UINT ulData1;
    UINT ulData2;
} payloadData;

void vPayloadStartTask();
payloadData* xPayloadGetData();

#endif	/* PAYLOAD_H */

