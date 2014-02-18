/* 
 * File:   power.h
 * Author: Kris
 *
 * Created on October 29, 2012, 10:07 AM
 */

#ifndef POWER_H
#define	POWER_H

#define powerLOCAL_PRINT

#if 0
/*
 * For internal memory management only.
 *   Bit field order is NOT guaranteed.
 *   Do NOT attempt to write this struct directly to disk, radio, etc.
 */
typedef struct {
    unsigned short
        XVoltage : 10,
        XCurrent0 : 10,
        XCurrent1 : 10,
        YVoltage : 10,
        YCurrent0 : 10,
        YCurrent1 : 10,
        ZVoltage : 10,
        ZCurrent0 : 10,
        ZCurrent1 : 10,
        BusBatteryCurrent : 10,
        Bus5VCurrent : 10,
        Bus3V3Current : 10,
        Battery0Temperature : 10,
        Battery0Voltage : 10,
        Battery0Direction : 1,
        Battery0Current : 10,
        Battery1Temperature : 10,
        Battery1Voltage : 10,
        Battery1Direction : 1,
        Battery1Current : 10;
} powerData;
#endif

#if 1
typedef struct {
    unsigned short
        XVoltage,
        XCurrent0,
        XCurrent1,
        YVoltage,
        YCurrent0,
        YCurrent1,
        ZVoltage,
        ZCurrent0,
        ZCurrent1,
        BusBatteryCurrent,
        Bus5VCurrent,
        Bus3V3Current,
        Battery0Temperature,
        Battery0Voltage,
        Battery0Direction,
        Battery0Current,
        Battery1Temperature,
        Battery1Voltage,
        Battery1Direction,
        Battery1Current;
} powerData;
#endif

void vPowerPrintHousekeepingData();
void vPowerPollHousekeepingData();
void vPowerStartTask();
powerData* xPowerGetHousekeepingData();

#endif	/* POWER_H */

