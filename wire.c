
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

/* The SCL output pin */
#define SCL         _RG2
/* The SCL Direction Register Bit */
#define SCL_TRIS    _TRISG2
/* The SDA output pin */
#define SDA         _RG3
/* The SDA Direction Register Bit */
#define SDA_TRIS    _TRISG3

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

static void vWireTestTask( void *pvParameters );
static void vWireTask( void *pvParameters );

char cWireSendByte(char byte);
char cWireGetByte(void);
char cWireGetAck(void);
void vWireSendAck(void);
void vWireDelay(char delay);
void vWireStart(void);
void vWireStop(void);
void vWireClock(void);
char cWireReadBit(void);

char cWireWrite( char cAddress, char *pcData, char cBytes )
{
    vWireStart();

    /* Send Address */
    cWireSendByte( cAddress & 0xFE );
    if( !cWireGetAck() )
    {
        vWireStop();
        return 0;
    }

    /* Send Data */
    while( cBytes-- )
    {
        cWireSendByte( *pcData );
        if( !cWireGetAck() )
        {
            vWireStop();
            return 0;
        }
        ++pcData;
    }

    vWireStop();
    return 1;
}

char cWireRead( char cAddress, char *pcData, char cBytes )
{
    vWireStart();

    /* Send Address */
    cWireSendByte( cAddress | 0x01 );
    if( !cWireGetAck() )
    {
        vWireStop();
        return 0;
    }

    /* Read Data */
    while( cBytes-- )
    {
        *pcData = cWireGetByte();
        pcData++;

        if( cBytes > 0 )
        {
            vWireSendAck();
        }
    }

    vWireStop();
    return 1;
}

char cWireSendByte( char cByte )
{
    char cCount;

    SDA = wireLOW;
    SCL = wireLOW;

    /* Give clock time to go low */
    vWireDelay(wireCLOCKLOW);

    /* Send 8 bits */
    for( cCount = 8; cCount; --cCount )
    {
        /* Get next bit from byte */
        if( (cByte & 0x80) == 0 )
        {
            /* Pull pin low */
            SDA = wireLOW;
            SDA_TRIS = wireLOW;
        }
        else
        {
            /* Release pin */
            SDA_TRIS = wireHIGH;
        }

        cByte = cByte << 1;
        
        vWireClock();
    }

    SDA_TRIS = wireHIGH;
    return 1;
}

char cWireGetByte()
{
    char cCount,
         cByte = 0;

    SDA = wireLOW;
    SCL = wireLOW;

    vWireDelay(wireCLOCKLOW);

    for( cCount = 8; cCount; --cCount )
    {
        cByte = cByte << 1;

        /* Release the pin */
        SDA_TRIS = wireHIGH;

        cByte |= cWireReadBit();
    }

    return cByte;
}

void vWireStart()
{
    /* Ensure pins are in high-Z mode */
    SDA_TRIS = wireHIGH;
    SCL_TRIS = wireHIGH;
    SCL = wireLOW;
    SDA = wireLOW;

    vWireDelay( wireSTARTDELAY );

    /* I2C Start condition */
    SDA_TRIS = wireLOW;
    SDA = wireLOW;
    vWireDelay( wireSTARTDELAY );
    SCL_TRIS = wireLOW;
    vWireDelay( wireCLOCKLOW );
}

void vWireStop()
{
    /* I2C Stop condition */
    SDA_TRIS = wireLOW;
    SCL_TRIS = wireHIGH;
    vWireDelay(wireSTOPDELAY);
    SDA_TRIS = wireHIGH;
}

void vWireClock()
{
    vWireDelay(wireDATASETTLE);
    SCL_TRIS = wireHIGH;
    vWireDelay(wireCLOCKHIGH);
    SCL_TRIS = wireLOW;
    vWireDelay(wireCLOCKLOW);
}

char cWireReadBit()
{
    char cBit = 0;

    vWireDelay(wireDATASETTLE);
    SCL_TRIS = wireHIGH;
    vWireDelay(wireHALFCLOCK);

    if( SDA != 0 )
    {
        cBit = 1;
    }

    vWireDelay(wireHALFCLOCK);
    SCL_TRIS = wireLOW;
    vWireDelay(wireCLOCKLOW);

    return cBit;
}

char cWireGetAck()
{
    SDA = wireLOW;
    SCL = wireLOW;

    /* Ensure clock is low */
    SCL_TRIS = wireLOW;
    /* Release the data pin so slave can ACK */
    SDA_TRIS = wireHIGH;
    /* Release the clock pin */
    SCL_TRIS = wireHIGH;

    vWireDelay(wireHALFCLOCK);
    if( SDA )
    {
        /* No ACK */
        return 0;
    }
    vWireDelay(wireHALFCLOCK);

    /* Finish the clock pulse */
    SCL_TRIS = wireLOW;
    vWireDelay(wireCLOCKLOW);
    vWireDelay(wireCLOCKLOW);

    return 1;
}

void vWireSendAck()
{
    SDA = 0;
    SDA_TRIS = wireLOW;
    vWireDelay(wireDATASETTLE);
    vWireClock();
    SDA_TRIS = wireHIGH;
    vWireDelay(wireDATASETTLE);
}

void vWireDelay(const char delay)
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

void vWireStartTestTask()
{
    xTaskCreate( vWireTestTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_WIRE + 1, NULL );
}


#define WRITE_9DOF 0xA6
#define READ_9DOF 0xA7

BOOL is9DofInit = 0;
static void vInit9Dof()
{
    char pcWireBuff[2]={0x2D,0x08};

    vConsolePuts("Initializing ADXL345...");

    // Setup ADXL345 for constant measurement mode
    is9DofInit = cWireWrite(WRITE_9DOF, pcWireBuff, 2);

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
    cWireWrite(WRITE_9DOF, cBuf, 1);
    cBuf[0] = 0;
    cWireRead(READ_9DOF, cBuf, 6);
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
    if( cWireWrite(addr,NULL,0) )
//    if( !i2c_write_byte(1,1,addr) )
    {
        static char out[20];
        sprintf(out, "Hit addr: 0x%x (0x%x Read)", addr, addr>>1);
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

static void vWireTestTask( void *pvParameters )
{
    char pcData[20];

    for( ;; )
    {
        vConsolePrint("\r\n");

        sprintf(pcData, "ZYXWVUTSRQPONML");

        vConsolePuts( "Test Write 0xA6" );
        if( cWireQueueWrite( wireBUS1, 0xA6, pcData, 20 ) )
        {
            vConsolePuts("Write success!");
        }
        else
        {
            vConsolePuts("Write failed!");
        }

        vConsolePuts( "Test Read 0xA7" );
        if( cWireQueueRead( wireBUS1, 0xA7, pcData, 20 ) )
        {
            vConsolePuts("Read success:");
            vConsolePuts(pcData);
        }
        else
        {
            vConsolePuts("Read failed!");
        }

        vTaskDelay(5000);
    }
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
#if 0
                if( cWireWrite( xMessage.cAddress, xMessage.pcData, xMessage.cBytes ) )
                {
                    *(xMessage.pcStatus) = wireSTATUS_SUCCESS;
                }
                else
                {
                    *(xMessage.pcStatus) = wireSTATUS_FAIL;
                }
#else
                char out[30];
                unsigned i;
                sprintf( out, "Write to 0x%x (%d bytes):", 0x00ff & (int) xMessage.cAddress, xMessage.cBytes);
                vConsolePuts( out );
                for( i=0; i<xMessage.cBytes-1 && i < 30-1; ++i )
                {
                    out[i] = xMessage.pcData[i];
                }
                out[i] = 0;
                vConsolePuts( out );
                vTaskDelay( 300 );
                vConsolePuts( "Done" );
                *(xMessage.pcStatus) = wireSTATUS_SUCCESS;
#endif
            }
            else
            {
                /* Read */
#if 0
                if( cWireRead( xMessage.cAddress, xMessage.pcData, xMessage.cBytes ) )
                {
                    *(xMessage.pcStatus) = wireSTATUS_SUCCESS;
                }
                else
                {
                    *(xMessage.pcStatus) = wireSTATUS_FAIL;
                }
#else
                char out[30];
                unsigned i;
                sprintf( out, "Read from 0x%x (%d bytes)", 0x00ff & (int) xMessage.cAddress, xMessage.cBytes);
                vConsolePuts( out );
                for( i=0; i<xMessage.cBytes-1; ++i )
                {
                    xMessage.pcData[i] = i+'A';
                }
                xMessage.pcData[i] = 0;
                vTaskDelay( 300 );
                vConsolePuts( "Done" );
                *(xMessage.pcStatus) = wireSTATUS_SUCCESS;
#endif
            }
        }
    }
}
