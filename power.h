/* 
 * File:   power.h
 * Author: Kris
 *
 * Created on October 29, 2012, 10:07 AM
 */

#ifndef POWER_H
#define	POWER_H

#define powerLOCAL_PRINT

typedef unsigned short ushort;

/*
 * For internal memory management only.
 *   Bit field order is NOT guaranteed.
 *   Do NOT attempt to write this struct directly to disk, radio, etc.
 */
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
    ushort BusBatteryCurrent : 10;
    ushort Bus5VCurrent : 10;
    ushort Bus3V3Current : 10;
    ushort Battery0Temperature : 10;
    ushort Battery0Voltage : 10;
    ushort Battery0Direction : 1;
    ushort Battery0Current : 10;
    ushort Battery1Temperature : 10;
    ushort Battery1Voltage : 10;
    ushort Battery1Direction : 1;
    ushort Battery1Current : 10;
} powerData;

void vPowerStartTask();
powerData* xPowerGetHousekeepingData();

#endif	/* POWER_H */

