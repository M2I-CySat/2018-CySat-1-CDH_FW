
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

char vWireWrite( char cAddress, char *pcData, char cBytes )
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

char vWireRead( char cAddress, char *pcData, char cBytes )
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
    unsigned char c = delay / 20;
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

BOOL is9DofInit = 0;
static void vInit9Dof()
{
    char I2CBUFF[2]={0x2D,0x08};

    is9DofInit = 1;

    vConsolePuts("Initializing ADXL345...");
    // Setup ADXL345 for constant measurement mode
//    vWireStart();
//    vWireWrite(WRITE_9DOF);
//    vWireWrite(0x2D);
//    vWireWrite(0x08);
//    vWireStop();

    vWireWrite(WRITE_9DOF, I2CBUFF, 2);

//    i2c_write_byte(1,0,WRITE_9DOF);
//    i2c_write_byte(0,0,0x2D);
//    i2c_write_byte(0,1,0x08);

    vConsolePuts("9DOF Initialized.");
}

static void vTest9Dof()
{
    static char str[35];
    char I2CBUFF[6] = {0, 0, 0, 0, 0, 0};
    int x = 0,
        y = 0,
        z = 0;

    if( !is9DofInit )
    {
        vInit9Dof();
    }

//    vWireStart();
//    vWireWrite(WRITE_9DOF);
//    vWireWrite(0x32);
//    vWireRestart();
//    vWireWrite(READ_9DOF);
//    vConsolePuts("baz");
//    x = ucWireRead() | (ucWireReadAndStop() << 8);
//    vConsolePuts("qux");
//
//    vWireStart();
//    vWireWrite(WRITE_9DOF);
//    vWireWrite(0x34);
//    vWireRestart();
//    vWireWrite(READ_9DOF);
//    y = ucWireRead() | (ucWireReadAndStop() << 8);
//
//    vWireStart();
//    vWireWrite(WRITE_9DOF);
//    vWireWrite(0x36);
//    vWireRestart();
//    vWireWrite(READ_9DOF);
//    z = ucWireRead() | (ucWireReadAndStop() << 8);

    I2CBUFF[0] = 0x32;
    vWireWrite(WRITE_9DOF, I2CBUFF, 1);
    I2CBUFF[0] = 0;
    vWireRead(READ_9DOF, I2CBUFF, 6);
    x = I2CBUFF[0] | (I2CBUFF[1] << 8);
    y = I2CBUFF[2] | (I2CBUFF[3] << 8);
    z = I2CBUFF[4] | (I2CBUFF[5] << 8);

//    I2CBUFF[0] = 0x32;
//    vWireWrite(WRITE_9DOF, I2CBUFF, 1);
//    vWireRead(READ_9DOF, I2CBUFF, 2);
//    x = I2CBUFF[0] | (I2CBUFF[1] << 8);
//
//    I2CBUFF[0] = 0x34;
//    vWireWrite(WRITE_9DOF, I2CBUFF, 1);
//    vWireRead(READ_9DOF, I2CBUFF, 2);
//    y = I2CBUFF[0] | (I2CBUFF[1] << 8);
//
//    I2CBUFF[0] = 0x36;
//    vWireWrite(WRITE_9DOF, I2CBUFF, 1);
//    vWireRead(READ_9DOF, I2CBUFF, 2);
//    z = I2CBUFF[0] | (I2CBUFF[1] << 8);

//    i2c_write_byte(1,0,WRITE_9DOF);
//    i2c_write_byte(0,1,0x32);
//    i2c_write_byte(1,0,READ_9DOF);
//    x = i2c_read_byte(0,0) | (i2c_read_byte(1,1) << 8);
//    i2c_write_byte(1,0,WRITE_9DOF);
//    i2c_write_byte(0,1,0x34);
//    i2c_write_byte(1,0,READ_9DOF);
//    y = i2c_read_byte(0,0) | (i2c_read_byte(1,1) << 8);
//    i2c_write_byte(1,0,WRITE_9DOF);
//    i2c_write_byte(0,1,0x36);
//    i2c_write_byte(1,0,READ_9DOF);
//    z = i2c_read_byte(0,0) | (i2c_read_byte(1,1) << 8);

//    sprintf(str, "[ADXL345] x=0x%04x, y=0x%04x, z=0x%04x", x, y, z);
    sprintf(str, "[ADXL345] x=%04d, y=%04d, z=%04d", x, y, z);
//    sprintf(str, "[ADXL345] x=0x%04x", x);
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
    if( vWireWrite(addr,NULL,0) )
//    if( !i2c_write_byte(1,1,addr) )
    {
        static char out[20];
        sprintf(out, "Hit addr: 0x%x (0x%x Read)", addr, addr>>1);
        vConsolePuts(out);
    }
    addr += 2;
}

static void vWireTask( void *pvParameters )
{
//    wireMessage xMessage;

    for( ;; )
    {
        vTest9Dof();
//        vSoftI2cScan();
        vTaskDelay(100);

//        if( xQueueReceive( xWireQueue, &xMessage, wireBLOCK_TIME ) )
//        {
//            xMessage.callback();
//        }
    }
}
