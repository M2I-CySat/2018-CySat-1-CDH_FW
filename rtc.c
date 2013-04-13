
#include "rtc.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "system.h"
#include "uart.h"
#include "wire.h"


#define rtcBUS  wireBUS1
#define rtcADDR 0xD0

static unsigned char ucConvertToInteger( char c );
static char cRtcEnable();
static void vRtcTask( void *pvParameters );


void vRtcStartTask()
{
    xTaskCreate( vRtcTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_TEST, NULL );
}

char cRtcRead( rtcTime *xTime )
{
    char pcData[8];

    /* First register address */
    pcData[0] = 0x00;
    if( 0 == cWireQueueWrite( rtcBUS, rtcADDR, pcData, 1 ) )
    {
        return 0;
    }

    if( 0 == cWireQueueRead( rtcBUS, rtcADDR, pcData, 8 ) )
    {
        return 0;
    }

    xTime->cCentiseconds =  ucConvertToInteger( pcData[0] );
    xTime->cSeconds =       ucConvertToInteger( pcData[1] & 0x7f );
    xTime->cMinutes =       ucConvertToInteger( pcData[2] );
    xTime->cHours =         ucConvertToInteger( pcData[3] & 0x3f );
    xTime->cDayOfWeek =     ucConvertToInteger( pcData[4] & 0x07 );
    xTime->cDayOfMonth =    ucConvertToInteger( pcData[5] & 0x3f );
    xTime->cMonth =         ucConvertToInteger( pcData[6] & 0x1f );
    xTime->cYear =          ucConvertToInteger( pcData[7] );

//    {
//        unsigned char i;
//        char out[10];
//
//        vConsolePrint( "Bytes: " );
//        vTaskDelay(100);
//        for( i=0; i<8; ++i )
//        {
//            sprintf( out, "%02x ", (unsigned char) pcData[i] );
//            vConsolePrint( out );
//            vTaskDelay(100);
//        }
//        vConsolePrint( "\r\n" );
//    }

    return 1;
}

void vRtcPrintTime()
{
    rtcTime xTime;

    if( cRtcRead( &xTime ) )
    {
        vConsolePrintf( "%02u:%02u:%02u.%02u\r\n", xTime.cHours, xTime.cMinutes, xTime.cSeconds, xTime.cCentiseconds );
    }
    else
    {
        vConsoleErrorPrintf( "RTC not found\r\n" );
    }
}

static char cRtcFlags( char *cFlags )
{
    char cReg = 0x0f;

    /* Request flags register */
    if( 0 == cWireQueueWrite( rtcBUS, rtcADDR, &cReg, 1 ) )
    {
        return 0;
    }
    if( 0 == cWireQueueRead( rtcBUS, rtcADDR, cFlags, 1 ) )
    {
        return 0;
    }

    return 1;
}

static char cRtcRestart()
{
    char cData[] = {0x01, 0x00};

    if( 0 == cWireQueueWrite( rtcBUS, rtcADDR, &cData[0], 1 ) )
    {
        return 0;
    }
    if( 0 == cWireQueueRead( rtcBUS, rtcADDR, &cData[1], 1 ) )
    {
        return 0;
    }

    /* Turn ST bit on */
    cData[1] |= 0x80;
    if( 0 == cWireQueueWrite( rtcBUS, rtcADDR, cData, 2 ) )
    {
        return 0;
    }

    /* Turn ST bit off */
    cData[1] &= ~0x7f;
    if( 0 == cWireQueueWrite( rtcBUS, rtcADDR, cData, 2 ) )
    {
        return 0;
    }

    return 1;
}

static char cRtcClearOF()
{
    char cData[] = {0x0f, 0x00};

    if( 0 == cWireQueueWrite( rtcBUS, rtcADDR, &cData[0], 1 ) )
    {
        return 0;
    }
    if( 0 == cWireQueueRead( rtcBUS, rtcADDR, &cData[1], 1 ) )
    {
        return 0;
    }

    cData[1] &= 0xeb;

    if( 0 == cWireQueueWrite( rtcBUS, rtcADDR, cData, 2 ) )
    {
        return 0;
    }

    return 1;
}

static char cRtcEnable()
{
    char cFlags;
    
    /* Request flags register */
    if( 0 == cRtcFlags( &cFlags ) )
    {
        vConsoleErrorPrintf( "RTC: I2C Failed\r\n" );
    }

    /* Check OF flag */
    if( 0 == (cFlags & 0x04) )
    {
        /* OF flag is not set */
        return 1;
    }

    vConsolePrint( "RTC: OF flag set, restarting RTC.\r\n" );

    /* Set and clear ST flag */
    cRtcRestart();

    /* Must delay at least 4 seconds */
    vTaskDelay(5000);

    /* Clear OF flag */
    cRtcClearOF();

    /* Confirm OF flag cleared */
    cRtcFlags( &cFlags );

    if( cFlags & 0x04 )
    {
        vConsoleErrorPrintf( "RTC: Restart failed\r\n" );
        return 0;
    }

    return 1;
}

static unsigned char ucConvertToInteger( char c )
{
    return (unsigned char) (0x0f & (c >> 4)) * 10 + (0x0f & c);
}

/* Test I2C reading and writing by assuming the RTC is not running */
static void vRtcTestI2C()
{
    unsigned char i;
    char pcDataOut[9] = {0x01,0xbe,0x73,0xab,0x55,0xfb,0x0f,0x9d,0x2a};
    char pcDataIn[8];

    vConsolePrint( "Testing I2C\r\n" );

    cWireQueueWrite( rtcBUS, rtcADDR, pcDataOut, 8 );
    cWireQueueWrite( rtcBUS, rtcADDR, pcDataOut, 1 );
    cWireQueueRead( rtcBUS, rtcADDR, pcDataIn, 7 );

    for( i=0; i<7; ++i )
    {
        vConsolePrintf( "%02x", pcDataIn[i] );
        vTaskDelay(100);
        if( pcDataOut[i+1] != pcDataIn[i] )
        {
            vConsoleErrorPrintf( "-Mismatch!\r\n" );
        }
    }
}

static void vRtcTask( void *pvParameters )
{
    vTaskDelay(1000);
    
    for( ;; )
    {
        vRtcTestI2C();
//        if( cRtcEnable() )
//        {
//            vRtcPrintTime();
//        }
        vTaskDelay(500);
    }
}
