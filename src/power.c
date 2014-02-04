
#include "power.h"
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "system.h"
#include "uart.h"
#include "wire.h"

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
    char channel[2] = {0, cChannel};

    if( wireSTATUS_SUCCESS != cWireWritePutsError( powerBUS, powerADDR, channel, 2 ) ){
        vConsoleErrorPrintf( "Power: I2C Write Error\r\n" );
        return 0;
    }

    vTaskDelay(2); //delay for power board read
    
    if( wireSTATUS_SUCCESS != cWireReadPutsError( powerBUS, powerADDR, pcData, 2 ) ){
        vConsoleErrorPrintf( "Power: I2C Read Error\r\n" );
        return 0;
    }

    /* If the EPS is not ready, it will send 0xf000, and we return 0 */
    return ( ( pcData[0] << 8 ) | ( 0xff & pcData[1] ) ) & 0x03ff;
}

void vPowerPollHousekeepingData()
{
    memset(&xHousekeepingData, 0, sizeof(powerData));

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
    vConsolePrintf( "%s: %f %s\r\n", pcName, fValue, pcUnit );
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
        vConsolePrint( "Bat 0 is DISCHARGING\r\n" );
    else
        vConsolePrint( "Bat 0 is CHARGING\r\n" );
    vPrintChannel("Bat 0 Current",xHousekeepingData.Battery0Current * -3.2 + 2926.22,"mA");

    vPrintChannel("Bat 1 Temp",xHousekeepingData.Battery1Temperature * -0.163 + 110.835,"degC");
    vPrintChannel("Bat 1 Voltage",xHousekeepingData.Battery1Voltage * -0.0939 + 9.71,"V");
    if( 0==xHousekeepingData.Battery1Voltage )
        vConsolePrint( "Bat 1 is DISCHARGING\r\n" );
    else
        vConsolePrint( "Bat 1 is CHARGING\r\n" );
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
        vTaskDelay(3000);
        vConsolePrint( "Power: Polling data.\r\n" );
        vPowerPollHousekeepingData();
#ifdef powerLOCAL_PRINT
        vTaskDelay(100);
        vPowerPrintHousekeepingData();
#endif
    }
}
