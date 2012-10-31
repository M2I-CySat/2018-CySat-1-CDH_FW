/* 
 * File:   power.h
 * Author: Kris
 *
 * Created on October 29, 2012, 10:07 AM
 */

#ifndef POWER_H
#define	POWER_H

typedef unsigned short ushort;

typedef struct {
    ushort XVoltage : 10;
    ushort XCurrent0 : 10;
    ushort XCurrent1 : 10;
    ushort YVoltage : 10;
    ushort YCurrent0 : 10;
    ushort YCurrent1 : 10;
    ushort ZVoltage : 10;
    ushort ZCurrent0 : 10;
    ushort ZCurrent1 : 10;
    ushort Battery0BusCurrent : 10;
    ushort Battery0Voltage : 10;
    ushort Battery0Direction : 1;
    ushort Battery0Current : 10;
    ushort Battery1BusCurrent : 10;
    ushort Battery1Voltage : 10;
    ushort Battery1Direction : 1;
    ushort Battery1Current : 10;
} powerData;

void vPowerStartTask();
powerData* xPowerGetHousekeepingData();

#endif	/* POWER_H */

