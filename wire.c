
/* Standard includes. */
#include <stdio.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include <i2c.h>

#include "system.h"
#include "uart.h"
#include "wire.h"

#define wireQUEUE_LENGTH    10
#define wireBLOCK_TIME      ( ( portTickType ) 0xffff )

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

static xQueueHandle xWireQueue;

static void vWireTask( void *pvParameters );

/* Internal write and read commands */
char cWireWrite( char cBus, char cAddress, char *pcData, char cBytes );
char cWireRead( char cBus, char cAddress, char *pcData, char cBytes );

char cWireSendByte( char cBus, char byte );
char cWireGetByte( char cBus );
char cWireGetAck( char cBus );
void vWireSendAck( char cBus );
void vWireStart( char cBus );
void vWireStop( char cBus );
void vWireClock( char cBus );
char cWireReadBit( char cBus );
void vWireDelay( char delay );

/* Bus selection and bit bashing */
/* Todo: Eventually, find a cleaner way to do this. */
void vBadBusError( char bus );
void vSetSCL(char bus, char val);
void vSetSDA(char bus, char val);
void vSetSCL_TRIS(char bus, char val);
void vSetSDA_TRIS(char bus, char val);
char cGetSCL(char bus);
char cGetSDA(char bus);

void vBadBusError( char bus )
{
    char out[50];
    sprintf( out, "Bad I2C Bus error: wireBUS%d not defined!", bus );
    vConsolePutsError( out );
}

void vSetSCL( char bus, char val )
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
void vSetSDA( char bus, char val )
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
void vSetSCL_TRIS( char bus, char val )
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
void vSetSDA_TRIS( char bus, char val )
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
char cGetSCL( char bus )
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
char cGetSDA( char bus )
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

char cWireWrite( char cBus, char cAddress, char *pcData, char cBytes )
{
    vWireStart( cBus );

    /* Send Address */
    cWireSendByte( cBus, cAddress & 0xFE );
    if( !cWireGetAck( cBus ) )
    {
        vWireStop( cBus );
        return 0;
    }

    /* Send Data */
    while( cBytes-- )
    {
        cWireSendByte( cBus, *pcData );
        if( !cWireGetAck( cBus ) )
        {
            vWireStop( cBus );
            return 0;
        }
        ++pcData;
    }

    vWireStop( cBus );
    return 1;
}

char cWireRead( char cBus, char cAddress, char *pcData, char cBytes )
{
    vWireStart( cBus );

    /* Send Address */
    cWireSendByte( cBus, cAddress | 0x01 );
    if( !cWireGetAck( cBus ) )
    {
        vWireStop( cBus );
        return 0;
    }

    /* Read Data */
    while( cBytes-- )
    {
        *pcData = cWireGetByte( cBus );
        pcData++;

        if( cBytes > 0 )
        {
            vWireSendAck( cBus );
        }
    }

    vWireStop( cBus );
    return 1;
}

char cWireSendByte( char cBus, char cByte )
{
    char cCount;

//    SDA = wireLOW;
    vSetSDA( cBus, wireLOW );
//    SCL = wireLOW;
    vSetSCL( cBus, wireLOW );

    /* Give clock time to go low */
    vWireDelay( wireCLOCKLOW );

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
        
        vWireClock( cBus );
    }

//    SDA_TRIS = wireHIGH;
    vSetSDA_TRIS( cBus, wireHIGH );
    return 1;
}

char cWireGetByte( char cBus )
{
    char cCount,
         cByte = 0;

//    SDA = wireLOW;
    vSetSDA( cBus, wireLOW );
//    SCL = wireLOW;
    vSetSCL( cBus, wireLOW );

    vWireDelay( wireCLOCKLOW );

    for( cCount = 8; cCount; --cCount )
    {
        cByte = cByte << 1;

        /* Release the pin */
//        SDA_TRIS = wireHIGH;
        vSetSDA_TRIS( cBus, wireHIGH );

        cByte |= cWireReadBit( cBus );
    }

    return cByte;
}

void vWireStart( char cBus )
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

    vWireDelay( wireSTARTDELAY );

    /* I2C Start condition */
//    SDA_TRIS = wireLOW;
    vSetSDA_TRIS( cBus, wireLOW );
//    SDA = wireLOW;
    vSetSDA( cBus, wireLOW );
    vWireDelay( wireSTARTDELAY );
//    SCL_TRIS = wireLOW;
    vSetSCL_TRIS( cBus, wireLOW );
    vWireDelay( wireCLOCKLOW );
}

void vWireStop( char cBus )
{
    /* I2C Stop condition */
//    SDA_TRIS = wireLOW;
    vSetSDA_TRIS( cBus, wireLOW );
//    SCL_TRIS = wireHIGH;
    vSetSCL_TRIS( cBus, wireHIGH );
    vWireDelay(wireSTOPDELAY);
//    SDA_TRIS = wireHIGH;
    vSetSDA_TRIS( cBus, wireHIGH );
}

void vWireClock( char cBus )
{
    vWireDelay(wireDATASETTLE);
//    SCL_TRIS = wireHIGH;
    vSetSCL_TRIS( cBus, wireHIGH );
    vWireDelay(wireCLOCKHIGH);
//    SCL_TRIS = wireLOW;
    vSetSCL_TRIS( cBus, wireLOW );
    vWireDelay(wireCLOCKLOW);
}

char cWireReadBit( char cBus )
{
    char cBit = 0;

    vWireDelay( wireDATASETTLE );
//    SCL_TRIS = wireHIGH;
    vSetSCL_TRIS( cBus, wireHIGH );
    vWireDelay( wireHALFCLOCK );

//    if( SDA != 0 )
    if ( cGetSDA( cBus ) != wireLOW )
    {
        cBit = 1;
    }

    vWireDelay(wireHALFCLOCK);
//    SCL_TRIS = wireLOW;
    vSetSCL_TRIS( cBus, wireLOW );
    vWireDelay(wireCLOCKLOW);

    return cBit;
}

char cWireGetAck( char cBus )
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

    vWireDelay( wireHALFCLOCK );
//    if( SDA )
    if( cGetSDA( cBus ) != wireLOW )
    {
        /* No ACK */
        return 0;
    }
    vWireDelay( wireHALFCLOCK );

    /* Finish the clock pulse */
//    SCL_TRIS = wireLOW;
    vSetSCL_TRIS( cBus, wireLOW );
    vWireDelay( wireCLOCKLOW );
    vWireDelay( wireCLOCKLOW );

    return 1;
}

void vWireSendAck( char cBus )
{
//    SDA = 0;
    vSetSDA( cBus, wireLOW );
//    SDA_TRIS = wireLOW;
    vSetSDA_TRIS( cBus, wireLOW );
    vWireDelay( wireDATASETTLE );
    vWireClock( cBus );
//    SDA_TRIS = wireHIGH;
    vSetSDA_TRIS( cBus, wireHIGH );
    vWireDelay( wireDATASETTLE );
}

void vWireDelay( const char delay )
{
    /* delay_us(delay) */
    unsigned char c = delay;
    while( c-- != 0 )
    {
        continue;
    }
}

void vWireStartTask()
{
    xWireQueue = xQueueCreate( wireQUEUE_LENGTH, ( unsigned portBASE_TYPE ) sizeof( wireMessage ) );
    xTaskCreate( vWireTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_WIRE, NULL );
}


#define WRITE_9DOF 0xA6
#define READ_9DOF 0xA7
#define BUS_9DOF wireBUS1

BOOL is9DofInit = 0;
static void vInit9Dof()
{
    char pcWireBuff[2]={0x2D,0x08};

    vConsolePuts("Initializing ADXL345...");

    // Setup ADXL345 for constant measurement mode
    is9DofInit = cWireWrite( BUS_9DOF, WRITE_9DOF, pcWireBuff, 2 );

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
    cWireWrite( BUS_9DOF, WRITE_9DOF, cBuf, 1 );
    cBuf[0] = 0;
    cWireRead( BUS_9DOF, READ_9DOF, cBuf, 6 );
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

#define READ_RTC 0xd0
static void vTestRtc()
{
//    i2c_write_byte(1,0,READ_RTC);

}

static void vSoftI2cScan()
{
    static unsigned char addr = 0x00;
    static char bus = wireBUS1;
    if( cWireWrite( bus, addr, NULL, 0) )
    {
        char out[30];
        sprintf(out, "Hit addr 0x%x (0x%x Read) on bus (%d)", addr, addr>>1, bus);
        vConsolePuts(out);
    }
    addr += 2;
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
                if( cWireWrite( xMessage.cBus, xMessage.cAddress, xMessage.pcData, xMessage.cBytes ) )
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
                if( cWireRead( xMessage.cBus, xMessage.cAddress, xMessage.pcData, xMessage.cBytes ) )
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
