
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "system.h"
#include "uart.h"
#include "wire.h"
#include "power.h"

#ifdef powerLOCAL_PRINT
#include <stdio.h>
#endif

#define powerBLOCK_TIME         ( ( portTickType ) 0xffff )
#define powerWIRE_BLOCK_TIME    ( ( portTickType ) 0xffff )

#define powerBUS    wireBUS1
#define powerADDR   ( 0x2D << 1 )

static powerData xHousekeepingData;

static void vPowerTask( void *pvParameters );

void vPowerPollHousekeepingData();

void vPowerStartTask()
{
    xTaskCreate( vPowerTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_POWER, NULL );
}

unsigned short usGetChannel( char cChannel )
{
    char pcData[2];

    if( wireSTATUS_SUCCESS != cWireWritePutsError( powerBUS, powerADDR, &cChannel, 1 ) ){
        vConsolePutsError( "Power: I2C Write Error" );
        return 0;
    }

    if( wireSTATUS_SUCCESS != cWireReadPutsError( powerBUS, powerADDR, pcData, 2 ) ){
        vConsolePutsError( "Power: I2C Read Error" );
        return 0;
    }

    /* If the EPS is not ready, it will send 0xf000, and we return 0 */
    return ( ( pcData[0] << 8 ) | ( 0xff & pcData[1] ) ) & 0x03ff;
}

void vPowerPollHousekeepingData()
{
    xHousekeepingData.XVoltage              = usGetChannel(  6 );
    xHousekeepingData.XCurrent0             = usGetChannel(  4 );
    xHousekeepingData.XCurrent1             = usGetChannel(  7 );
    xHousekeepingData.YVoltage              = usGetChannel(  3 );
    xHousekeepingData.YCurrent0             = usGetChannel(  1 );
    xHousekeepingData.YCurrent1             = usGetChannel( 13 );
    xHousekeepingData.ZVoltage              = usGetChannel(  9 );
    xHousekeepingData.ZCurrent0             = usGetChannel( 10 );
    xHousekeepingData.ZCurrent1             = usGetChannel( 31 );
    xHousekeepingData.BusBatteryCurrent     = usGetChannel( 17 );
    xHousekeepingData.Bus5VCurrent          = usGetChannel( 26 );
    xHousekeepingData.Bus3V3Current         = usGetChannel( 27 );
    xHousekeepingData.Battery0Temperature   = usGetChannel( 23 );
    xHousekeepingData.Battery0Voltage       = usGetChannel( 24 );
    xHousekeepingData.Battery0Direction     = usGetChannel( 28 ) & 0x01;
    xHousekeepingData.Battery0Current       = usGetChannel( 29 );
    xHousekeepingData.Battery1Temperature   = usGetChannel( 18 );
    xHousekeepingData.Battery1Voltage       = usGetChannel( 19 );
    xHousekeepingData.Battery1Direction     = usGetChannel( 21 ) & 0x01;
    xHousekeepingData.Battery1Current       = usGetChannel( 22 );
}

#ifdef powerLOCAL_PRINT
static void vPrintChannel( char* pcName, double fValue, char* pcUnit )
{
    char pc[15];

    vConsolePrint( pcName );
    vTaskDelay(10);
    vConsolePrint( ": " );
    vTaskDelay(10);
    sprintf( pc, "%f ", fValue );
    vConsolePrint( pc );
    vTaskDelay(10);
    vConsolePuts( pcUnit );
    vTaskDelay(10);
}

void vPowerPrintHousekeepingData()
{
    vPrintChannel("X Voltage",xHousekeepingData.XVoltage * -0.0086 + 8.81,"V");
    vPrintChannel("-X Current",xHousekeepingData.XCurrent0 * -0.5 + 515.7,"mA");
    vPrintChannel("+X Current",xHousekeepingData.XCurrent1 * -0.5 + 515.7,"mA");
    vPrintChannel("Y Voltage",xHousekeepingData.YVoltage * -0.0086 + 8.81,"V");
    vPrintChannel("-Y Current",xHousekeepingData.YCurrent0 * -0.5 + 515.7,"mA");
    vPrintChannel("+Y Current",xHousekeepingData.YCurrent1 * -0.5 + 515.7,"mA");
    vPrintChannel("Z Voltage",xHousekeepingData.ZVoltage * -0.0086 + 8.81,"V");
    vPrintChannel("-Z Current",xHousekeepingData.ZCurrent0 * -0.5 + 515.7,"mA");
    vPrintChannel("+Z Current",xHousekeepingData.ZCurrent1 * -0.5 + 515.7,"mA");

    vPrintChannel("Bat Bus Current",xHousekeepingData.BusBatteryCurrent * -3.153 + 3250.815,"mA");
    vPrintChannel("5V Bus Current",xHousekeepingData.Bus5VCurrent * -3.5 + 3611.509,"mA");
    vPrintChannel("3.3V Bus Current",xHousekeepingData.Bus3V3Current * -4.039 + 4155.271,"mA");

    vPrintChannel("Bat 0 Temp",xHousekeepingData.Battery0Temperature * -0.163 + 110.835,"degC");
    vPrintChannel("Bat 0 Voltage",xHousekeepingData.Battery0Voltage * -0.0939 + 9.71,"V");
    if( 0==xHousekeepingData.Battery0Voltage )
        vConsolePuts( "Bat 0 is DISCHARGING" );
    else
        vConsolePuts( "Bat 0 is CHARGING" );
    vPrintChannel("Bat 0 Current",xHousekeepingData.Battery0Current * -3.2 + 2926.22,"mA");

    vPrintChannel("Bat 1 Temp",xHousekeepingData.Battery1Temperature * -0.163 + 110.835,"degC");
    vPrintChannel("Bat 1 Voltage",xHousekeepingData.Battery1Voltage * -0.0939 + 9.71,"V");
    if( 0==xHousekeepingData.Battery1Voltage )
        vConsolePuts( "Bat 1 is DISCHARGING" );
    else
        vConsolePuts( "Bat 1 is CHARGING" );
    vPrintChannel("Bat 1 Current",xHousekeepingData.Battery1Current * -3.2 + 2926.22,"mA");
}
#endif

powerData* xPowerGetHousekeepingData()
{
    return &xHousekeepingData;
}

static void vPowerTask( void *pvParameters )
{

    for( ;; )
    {
        vTaskDelay(1000);
        vConsolePuts("Power: Polling data.");
        vPowerPollHousekeepingData();
#ifdef powerLOCAL_PRINT
        vTaskDelay(100);
        vPowerPrintHousekeepingData();
#endif
    }
}
