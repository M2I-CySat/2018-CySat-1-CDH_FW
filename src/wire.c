
#include "wire.h"

/* Standard includes. */

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"

//#include <i2c.h>

#include "system.h"
#include "uart.h"

#define wireQUEUE_LENGTH    10
#define wireBLOCK_TIME      ( ( portTickType ) 0xffff )

#ifndef NULL
#define NULL 0 //wtf
#endif

/* SCL*:        The SCL output pin */
/* SCL*_TRIS:   The SCL Direction Register Bit */
/* SDA*:        The SDA output pin */
/* SDA*_TRIS:   The SDA Direction Register Bit */

#define SCL1         _RG2
#define SCL1_TRIS    _TRISG2
#define SDA1         _RG3
#define SDA1_TRIS    _TRISG3

#define SCL2         _RA2
#define SCL2_TRIS    _TRISA2
#define SDA2         _RA3
#define SDA2_TRIS    _TRISA3

#define SCL3         _RE6
#define SCL3_TRIS    _TRISE6
#define SDA3         _RE7
#define SDA3_TRIS    _TRISE7

#define SCL4         _RB2
#define SCL4_TRIS    _TRISB2
#define SDA4         _RB3
#define SDA4_TRIS    _TRISB3

#define wireLOW     0
#define wireHIGH    1

/* I2C Bus Timing - usec */
#define wireSTARTDELAY 50
#define wireSTOPDELAY  50
#define wireDATASETTLE 20
#define wireCLOCKHIGH  100
#define wireHALFCLOCK  50
#define wireCLOCKLOW   100
#define wireACKWAITMIN 50

/* Hacky way to adjust how long 1us is */
#define wireDELAY_SCALE 10

static xQueueHandle xWireQueue;

xQueueHandle xMutexBus1;
xQueueHandle xMutexBus2;
xQueueHandle xMutexBus3;
xQueueHandle xMutexBus4;

static void vWireTask( void *pvParameters );

/* Private function prototypes */
static char cWrite( char cBus, char cAddress, char *pcData, char cBytes );
static char cRead( char cBus, char cAddress, char *pcData, char cBytes );

static char cSendByte( char cBus, char byte );
static char cGetByte( char cBus );
static char cGetAck( char cBus );
static void vSendAck( char cBus );
static void vStart( char cBus );
static void vStop( char cBus );
static void vClock( char cBus );
static char cReadBit( char cBus );
static void vDelay( char delay );

/* Bus selection and bit bashing */
/* Todo: Eventually, find a cleaner way to do this. */
static void vBadBusError( char bus );
static void vSetSCL(char bus, char val);
static void vSetSDA(char bus, char val);
static void vSetSCL_TRIS(char bus, char val);
static void vSetSDA_TRIS(char bus, char val);
static char cGetSCL(char bus);
static char cGetSDA(char bus);

static void vSendNack(char bus);

static char xMutexTakeBus( char cBus );
static char xMutexGiveBus( char cBus );

/* TODO: Change switch's cBus = 1,2,3,4 to the defined values wireBUS1, etc. */

static char xMutexTakeBus( char cBus )
{
    switch( cBus )
    {
        case 1: return xSemaphoreTake( xMutexBus1, wireBLOCK_TIME );
        case 2: return xSemaphoreTake( xMutexBus2, wireBLOCK_TIME );
        case 3: return xSemaphoreTake( xMutexBus3, wireBLOCK_TIME );
        case 4: return xSemaphoreTake( xMutexBus4, wireBLOCK_TIME );
        default: vBadBusError( cBus );
    }

    return pdFALSE;
}

static char xMutexGiveBus( char cBus )
{
    switch( cBus )
    {
        case 1: return xSemaphoreGive( xMutexBus1 );
        case 2: return xSemaphoreGive( xMutexBus2 );
        case 3: return xSemaphoreGive( xMutexBus3 );
        case 4: return xSemaphoreGive( xMutexBus4 );
        default: vBadBusError( cBus );
    }

    return pdFALSE;
}

static void vBadBusError( char bus )
{
    vConsoleErrorPrintf( "Bad I2C Bus error: bus %d not defined!\r\n", bus );
}

static void vSetSCL( char bus, char val )
{
    switch( bus )
    {
        case 1: SCL1 = val; return;
        case 2: SCL2 = val; return;
        case 3: SCL3 = val; return;
        case 4: SCL4 = val; return;
        default: vBadBusError( bus );
    }
}
static void vSetSDA( char bus, char val )
{
    switch( bus )
    {
        case 1: SDA1 = val; return;
        case 2: SDA2 = val; return;
        case 3: SDA3 = val; return;
        case 4: SDA4 = val; return;
        default: vBadBusError( bus );
    }
}
static void vSetSCL_TRIS( char bus, char val )
{
    switch( bus )
    {
        case 1: SCL1_TRIS = val; return;
        case 2: SCL2_TRIS = val; return;
        case 3: SCL3_TRIS = val; return;
        case 4: SCL4_TRIS = val; return;
        default: vBadBusError( bus );
    }
}
static void vSetSDA_TRIS( char bus, char val )
{
    switch( bus )
    {
        case 1: SDA1_TRIS = val; return;
        case 2: SDA2_TRIS = val; return;
        case 3: SDA3_TRIS = val; return;
        case 4: SDA4_TRIS = val; return;
        default: vBadBusError( bus );
    }
}
static char cGetSCL( char bus )
{
    switch( bus )
    {
        case 1: return SCL1;
        case 2: return SCL2;
        case 3: return SCL3;
        case 4: return SCL4;
        default: vBadBusError( bus );
    }
    return wireHIGH;
}
static char cGetSDA( char bus )
{
    switch( bus )
    {
        case 1: return SDA1;
        case 2: return SDA2;
        case 3: return SDA3;
        case 4: return SDA4;
        default: vBadBusError( bus );
    }
    return wireHIGH;
}

static char cWrite( char cBus, char cAddress, char *pcData, char cBytes )
{
    vStart( cBus );

    /* Check if SCL is not held down */
    if( !cGetSCL( cBus ) )
    {
        return wireSTATUS_HELD_LOW;
    }

    /* Send Address */
    cSendByte( cBus, cAddress & 0xFE );
    if( !cGetAck( cBus ) )
    {
        vStop( cBus );
        return wireSTATUS_FAIL;
    }

    /* Send Data */
    while( cBytes-- )
    {
        cSendByte( cBus, *pcData );
        if( !cGetAck( cBus ) )
        {
            vStop( cBus );
            return wireSTATUS_NACK;
        }
        ++pcData;
    }

    vStop( cBus );
    return wireSTATUS_SUCCESS;
}

static char cRead( char cBus, char cAddress, char *pcData, char cBytes )
{
    vStart( cBus );

    /* Check if SCL is not held down */
    if( !cGetSCL( cBus ) )
    {
        return wireSTATUS_HELD_LOW;
    }

    /* Send Address */
    cSendByte( cBus, cAddress | 0x01 );
    if( !cGetAck( cBus ) )
    {
        vStop( cBus );
        return wireSTATUS_NACK;
    }

    /* Read Data */
    while( cBytes-- )
    {
        *pcData = cGetByte( cBus );
        pcData++;

        if( cBytes > 0 )
        {
            vSendAck( cBus );
        }
    }

    vSendNack(cBus);
    vStop( cBus );
    
    return wireSTATUS_SUCCESS;
}

static char cSendByte( char cBus, char cByte )
{
    char cCount;

//    SDA = wireLOW;
    vSetSDA( cBus, wireLOW );
//    SCL = wireLOW;
    vSetSCL( cBus, wireLOW );

    /* Give clock time to go low */
    vDelay( wireCLOCKLOW );

    /* Send 8 bits */
    for( cCount = 8; cCount; --cCount )
    {
        /* Get next bit from byte */
        if( (cByte & 0x80) == 0 )
        {
            /* Pull pin low */
//            SDA = wireLOW;
            vSetSDA( cBus, wireLOW );
//            SDA_TRIS = wireLOW;
            vSetSDA_TRIS( cBus, wireLOW );
        }
        else
        {
            /* Release pin */
//            SDA_TRIS = wireHIGH;
            vSetSDA_TRIS( cBus, wireHIGH );
        }

        cByte = cByte << 1;
        
        vClock( cBus );
    }

//    SDA_TRIS = wireHIGH;
    vSetSDA_TRIS( cBus, wireHIGH );
    return wireSTATUS_SUCCESS;
}

static char cGetByte( char cBus )
{
    char cCount,
         cByte = 0;

//    SDA = wireLOW;
    vSetSDA( cBus, wireLOW );
//    SCL = wireLOW;
    vSetSCL( cBus, wireLOW );

    vDelay( wireCLOCKLOW );

    for( cCount = 8; cCount; --cCount )
    {
        cByte = cByte << 1;

        /* Release the pin */
//        SDA_TRIS = wireHIGH;
        vSetSDA_TRIS( cBus, wireHIGH );

        cByte |= cReadBit( cBus );
    }

    return cByte;
}

static void vStart( char cBus )
{
    /* Ensure pins are in high-Z mode */
//    SDA_TRIS = wireHIGH;
    vSetSDA_TRIS( cBus, wireHIGH );
//    SCL_TRIS = wireHIGH;
    vSetSCL_TRIS( cBus, wireHIGH );
//    SCL = wireLOW;
    vSetSCL( cBus, wireLOW );
//    SDA = wireLOW;
    vSetSDA( cBus, wireLOW );

    vDelay( wireSTARTDELAY );

    /* I2C Start condition */
//    SDA_TRIS = wireLOW;
    vSetSDA_TRIS( cBus, wireLOW );
//    SDA = wireLOW;
    vSetSDA( cBus, wireLOW );
    vDelay( wireSTARTDELAY );
//    SCL_TRIS = wireLOW;
    vSetSCL_TRIS( cBus, wireLOW );
    vDelay( wireCLOCKLOW );
}

static void vStop( char cBus )
{
    /* I2C Stop condition */
//    SDA_TRIS = wireLOW;
    vSetSDA_TRIS( cBus, wireLOW );
//    SCL_TRIS = wireHIGH;
    vSetSCL_TRIS( cBus, wireHIGH );
    vDelay(wireSTOPDELAY);
//    SDA_TRIS = wireHIGH;
    vSetSDA_TRIS( cBus, wireHIGH );

}

static void vSendNack( char cBus )
{
//    SDA = 0;
    vSetSDA( cBus, wireLOW );
//    SDA_TRIS = wireLOW;
    vSetSDA_TRIS( cBus, wireHIGH );
    vDelay( wireDATASETTLE );
    vClock( cBus );
//    SDA_TRIS = wireHIGH;
    vDelay( wireDATASETTLE );
}

static void vClock( char cBus )
{
    vDelay(wireDATASETTLE);
//    SCL_TRIS = wireHIGH;
    vSetSCL_TRIS( cBus, wireHIGH );
    vDelay(wireCLOCKHIGH);
//    SCL_TRIS = wireLOW;
    vSetSCL_TRIS( cBus, wireLOW );
    vDelay(wireCLOCKLOW);
}

static char cReadBit( char cBus )
{
    char cBit = 0;

    vDelay( wireDATASETTLE );
//    SCL_TRIS = wireHIGH;
    vSetSCL_TRIS( cBus, wireHIGH );
    vDelay( wireHALFCLOCK );

//    if( SDA != 0 )
    if ( cGetSDA( cBus ) != wireLOW )
    {
        cBit = 1;
    }

    vDelay(wireHALFCLOCK);
//    SCL_TRIS = wireLOW;
    vSetSCL_TRIS( cBus, wireLOW );
    vDelay(wireCLOCKLOW);

    return cBit;
}

static char cGetAck( char cBus )
{
//    SDA = wireLOW;
    vSetSDA( cBus, wireLOW );
//    SCL = wireLOW;
    vSetSCL( cBus, wireLOW );

    /* Ensure clock is low */
//    SCL_TRIS = wireLOW;
    vSetSCL_TRIS( cBus, wireLOW );
    /* Release the data pin so slave can ACK */
//    SDA_TRIS = wireHIGH;
    vSetSDA_TRIS( cBus, wireHIGH );
    /* Release the clock pin */
//    SCL_TRIS = wireHIGH;
    vSetSCL_TRIS( cBus, wireHIGH );

    vDelay( wireHALFCLOCK );
//    if( SDA )
    if( cGetSDA( cBus ) != wireLOW )
    {
        /* No ACK */
        return 0;
    }
    vDelay( wireHALFCLOCK );

    /* Finish the clock pulse */
//    SCL_TRIS = wireLOW;
    vSetSCL_TRIS( cBus, wireLOW );
    vDelay( wireCLOCKLOW );
    vDelay( wireCLOCKLOW );

    return 1;
}

static void vSendAck( char cBus )
{
//    SDA = 0;
    vSetSDA( cBus, wireLOW );
//    SDA_TRIS = wireLOW;
    vSetSDA_TRIS( cBus, wireLOW );
    vDelay( wireDATASETTLE );
    vClock( cBus );
//    SDA_TRIS = wireHIGH;
    vSetSDA_TRIS( cBus, wireHIGH );
    vDelay( wireDATASETTLE );
}

static void vDelay( const char delay )
{
    /* delay_us(delay) */
    /* TODO: Implement this correctly */
    unsigned char c, d;
    for( c=delay; c; --c )
    {
        for( d=wireDELAY_SCALE; d; --d );
    }
}

void vWireStartTask()
{
    xWireQueue = xQueueCreate( wireQUEUE_LENGTH, ( unsigned portBASE_TYPE ) sizeof( wireMessage ) );
    xTaskCreate( vWireTask, NULL, configMINIMAL_STACK_SIZE + 100, NULL, systemPRIORITY_WIRE, NULL );

    vWireInit();
}

void vWireInit()
{
    static char isInitialized = 0;
    if( 0 == isInitialized ) {
        xMutexBus1 = xSemaphoreCreateMutex();
        xMutexBus2 = xSemaphoreCreateMutex();
        xMutexBus3 = xSemaphoreCreateMutex();
        xMutexBus4 = xSemaphoreCreateMutex();
        isInitialized = 1;
    }
}

#ifdef TEST_9DOF
#define WRITE_9DOF 0xA6
#define READ_9DOF 0xA7
#define BUS_9DOF wireBUS1

BOOL is9DofInit = 0;
static void vInit9Dof()
{
    char pcWireBuff[2]={0x2D,0x08};

    vConsolePuts("Initializing ADXL345...");

    // Setup ADXL345 for constant measurement mode
    is9DofInit = cWrite( BUS_9DOF, WRITE_9DOF, pcWireBuff, 2 );

    if( is9DofInit )
    {
        vConsolePuts("Done.");
    }
    else
    {
        vConsolePuts("I2C Error");
    }
    
}

static void vTest9Dof()
{
    static char str[40];
    char cBuf[6] = {0, 0, 0, 0, 0, 0};
    unsigned char* ucBuf = (unsigned char*) cBuf;
    int x = 0,
        y = 0,
        z = 0;

    if( !is9DofInit )
    {
        vInit9Dof();
        return;
    }

    cBuf[0] = 0x32;
    cWrite( BUS_9DOF, WRITE_9DOF, cBuf, 1 );
    cBuf[0] = 0;
    cRead( BUS_9DOF, READ_9DOF, cBuf, 6 );
//    x = (unsigned) ucBuf[0] | ((unsigned) ucBuf[1] << 8);
//    y = (unsigned) ucBuf[2] | ((unsigned) ucBuf[3] << 8);
//    z = (unsigned) ucBuf[4] | ((unsigned) ucBuf[5] << 8);
    x = *((int*) &ucBuf[0]);
    y = *((int*) &ucBuf[2]);
    z = *((int*) &ucBuf[4]);

    /* Read bytes as signed! */
//    sprintf(str, "[ADXL345] x=0x%04x, y=0x%04x, z=0x%04x", x, y, z);
    sprintf(str, "[ADXL345] %04x%04x%04x -> x=%4d, y=%4d, z=%4d",
//            *((unsigned*) &ucBuf[0]), *((unsigned*) &ucBuf[2]), *((unsigned*) &ucBuf[4]),
//            *((unsigned*) &ucBuf[0]), *((unsigned*) &ucBuf[2]), *((unsigned*) &ucBuf[4]));
            ucBuf[0]<<8 | ucBuf[1], ucBuf[2]<<8 | ucBuf[3], ucBuf[4]<<8 | ucBuf[5], /* force endianness */
            x, y, z);
//    sprintf(str, "[ADXL345] x=%+03d, y=%+03d, z=%+03d", x, y, z);
    vConsolePuts(str);
}
#endif

static void vSoftI2cScan()
{
    static unsigned char addr = 0x00;
    static char bus = wireBUS1;
    if( cWrite( bus, addr, NULL, 0) )
    {
        vConsolePrintf( "Hit addr 0x%x (0x%x Read) on bus (%d)\r\n", addr, addr>>1, bus );
    }
    addr += 2;
}

void vWireScan( char cBus )
{
    char addr = 0x00;

    vConsolePrint( "Scanning I2C\r\n" );
    vTaskDelay(100);
    vConsolePrintf( "Bus: %d", (int) cBus );

    do
    {
//        if( cWireQueueAdd( bus, addr, NULL, 0 ) )
        if( wireSTATUS_SUCCESS == cWireRead( cBus, addr, NULL, 0 ) )
        {
//            int i;
//            for( i=0; i<40; ++i ) out[i] = 0;
//            sprintf( out, "Bus %d Addr %02x (%02xR) ACK", cBus, 0xff & addr, 0x7f & (addr >> 1) );
//            vConsolePuts( out );
//            vTaskDelay(10);
            vConsolePrintf( "%02x Hit!\r\n" );
        }
        addr += 2;

    } while( addr != 0x00 );
}

char cWireWrite( char cBus, char cAddress, char *pcData, char cBytes )
{
    char ret = 0;

    if( xMutexTakeBus( cBus ) )
    {
        ret = cWrite( cBus, cAddress, pcData, cBytes );
        xMutexGiveBus( cBus );
    }

    return ret;
}

char cWireRead( char cBus, char cAddress, char *pcData, char cBytes )
{
    char ret = 0;

    if( xMutexTakeBus( cBus ) )
    {
        ret = cRead( cBus, cAddress, pcData, cBytes );
        xMutexGiveBus( cBus );
    }

    return ret;
}

char cWirePutsStatus( char cStatus )
{
    switch( cStatus )
    {
        case wireSTATUS_SUCCESS:
            break;
        case wireSTATUS_DEFAULT:
            /* Should never ever happen */
            vConsoleErrorPrintf( wireERROR_PREFIX wireSTATUS_DEFAULT_MESSAGE "\r\n" );
            break;
        case wireSTATUS_FAIL:
            vConsoleErrorPrintf( wireERROR_PREFIX wireSTATUS_FAIL_MESSAGE "\r\n" );
            break;
        case wireSTATUS_NACK:
            vConsoleErrorPrintf( wireERROR_PREFIX wireSTATUS_NACK_MESSAGE "\r\n"  );
            break;
        case wireSTATUS_HELD_LOW:
            vConsoleErrorPrintf( wireERROR_PREFIX wireSTATUS_HELD_LOW_MESSAGE "\r\n"  );
            break;
    }
    return cStatus;
}

char cWireWritePutsError( char cBus, char cAddress, char *pcData, char cBytes )
{
    char status = cWireWrite( cBus, cAddress, pcData, cBytes );
    if( wireSTATUS_SUCCESS != status )
    {
//        char pcError[40];
//        sprintf( pcError, "I2C Write Error (bus %d, addr %02x)", (unsigned char) cBus, (unsigned char) cAddress );
//        vConsolePutsError( pcError );
        cWirePutsStatus(status);
    }
    return status;
}

char cWireReadPutsError( char cBus, char cAddress, char *pcData, char cBytes )
{
    char status = cWireRead( cBus, cAddress, pcData, cBytes );
    if( wireSTATUS_SUCCESS != status )
    {
//        char pcError[40];
//        sprintf( pcError, "I2C Read Error (bus %d, addr %02x)", (unsigned char) cBus, (unsigned char) cAddress );
//        vConsolePutsError( pcError );
        cWirePutsStatus(status);
    }
    return status;
}

char cWireQueueAdd( char cBus, char cAddress, char *pcData, char cBytes )
{
    wireMessage xMessage;
    char pcStatus = wireSTATUS_DEFAULT;

    xMessage.cBus = cBus;
    xMessage.cAddress = cAddress;
    xMessage.pcData = pcData;
    xMessage.cBytes = cBytes;
    xMessage.pcStatus = &pcStatus;

    if( xQueueSend( xWireQueue, &xMessage, wireBLOCK_TIME ) != pdPASS )
    {
        return wireSTATUS_FAIL;
    }

    /* TODO: Make this interrupt-based instead of polling */
    while ( wireSTATUS_DEFAULT == pcStatus )
    {
        vTaskDelay( 100 );
    }

    return ( wireSTATUS_SUCCESS == pcStatus );
}

char cWireQueueWrite( char cBus, char cAddress, char *pcData, char cBytes )
{
    /* Ensure read bit is cleared */
    cAddress &= 0xfe;
    return cWireQueueAdd( cBus, cAddress, pcData, cBytes );
}

char cWireQueueRead( char cBus, char cAddress, char *pcData, char cBytes )
{
    /* Ensure read bit is set */
    cAddress |= 0x01;
    return cWireQueueAdd( cBus, cAddress, pcData, cBytes );
}


static void vWireTask( void *pvParameters )
{
    wireMessage xMessage;

    for( ;; )
    {
//        vTest9Dof();
//        vSoftI2cScan();
        vTaskDelay(200);

        if( xQueueReceive( xWireQueue, &xMessage, wireBLOCK_TIME ) )
        {
            /* Check if read bit is cleared */
            if( 0 == ( xMessage.cAddress & 0x01 ) )
            {
                /* Write */
                if( cWrite( xMessage.cBus, xMessage.cAddress, xMessage.pcData, xMessage.cBytes ) )
                {
                    *(xMessage.pcStatus) = wireSTATUS_SUCCESS;
                }
                else
                {
                    *(xMessage.pcStatus) = wireSTATUS_FAIL;
                }
            }
            else
            {
                /* Read */
                if( cRead( xMessage.cBus, xMessage.cAddress, xMessage.pcData, xMessage.cBytes ) )
                {
                    *(xMessage.pcStatus) = wireSTATUS_SUCCESS;
                }
                else
                {
                    *(xMessage.pcStatus) = wireSTATUS_FAIL;
                }
            }
        }
    }
}
