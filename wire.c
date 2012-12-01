
/* Standard includes. */
#include <stdio.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include <i2c.h>

#include "system.h"
#include "uart.h"
#include "soft_i2c.h"
#include "i2c_wiki.h"
#include "wire.h"

#define wireQUEUE_LENGTH    10
#define wireBLOCK_TIME      ( ( portTickType ) 0xffff )

static xQueueHandle xWireQueue;

static void vWireTask( void *pvParameters );

void vWireInit()
{

    UINT config1 = 0;
    UINT config2 = 0;

    TRISAbits.TRISA2 = 1;

    // Turn off I2C modules
    CloseI2C2(); //Disbale I2C2 mdolue if enabled previously

    // I2C interrupt configuration
    ConfigIntI2C2(MI2C_INT_OFF); //Disable I2C interrupt

    // I2C2 configuration
    //    I2C2 enabled
    //    continue I2C module in Idle mode
    //    IPMI mode not enabled
    //    I2CADD is 7-bit address
    //    Disable Slew Rate Control for 100KHz
    //    Enable SM bus specification
    //    Disable General call address
    //    Baud @ 8MHz = 39 into I2CxBRG

//    config1 = (I2C_ON & I2C_7BIT_ADD);
    config2 = 39;

//    // Baud rate is set for 100 kHz
//    config2 = 0x11;
    // Configure I2C for 7 bit address mode
    config1 = (I2C_ON & I2C_IDLE_CON & I2C_CLK_HLD &
               I2C_IPMI_DIS & I2C_7BIT_ADD &
               I2C_SLW_DIS & I2C_SM_DIS &
               I2C_GCALL_DIS & I2C_STR_DIS &
               I2C_NACK & I2C_ACK_DIS & I2C_RCV_DIS &
               I2C_STOP_DIS & I2C_RESTART_DIS &
               I2C_START_DIS);

    OpenI2C2(config1, config2); //configure I2C2
}

void vWireStart()
{
    IdleI2C2(); //wait for the I2C to be Idle
    StartI2C2();
    while (I2C2CONbits.SEN); //Wait till Start sequence is completed
}

void vWireRestart()
{
    IdleI2C2(); //wait for the I2C to be Idle
    RestartI2C2(); //Restart signal
    while (I2C2CONbits.RSEN); //Wait till Restart sequence is completed
}

void vWireStop()
{
    IdleI2C2(); //wait for the I2C to be Idle
    StopI2C2(); //Terminate communication protocol with stop signal
    while (I2C2CONbits.PEN); //Wait till stop sequence is completed
    IdleI2C2();
}

void vWireWait()
{
    UINT i;
    for (i = 0; i < 1000; i++);
    Nop();
}

void vWireWrite(BYTE data)
{
    I2C2ADD=0x00;
    MasterWriteI2C2(data);
    I2C2ADD=0x00;
    while (I2C2STATbits.TBF); //Wait till address is transmitted
    while (!_MI2C2IF); //Wait for ninth clock cycle
    while (I2C2STATbits.ACKSTAT);
}

BYTE ucWireRead()
{
    BYTE r = MasterReadI2C2();
    while (!_MI2C2IF); //Wait for ninth clock cycle
    AckI2C2();
    return r;
}

BYTE ucWireReadAndStop()
{
    BYTE r = MasterReadI2C2();
    while (!_MI2C2IF); //Wait for ninth clock cycle
    NotAckI2C2();
    vWireStop();
    return r;
}

void vWireStartTask()
{
    xWireQueue = xQueueCreate( wireQUEUE_LENGTH, ( unsigned portBASE_TYPE ) sizeof( wireMessage ) );
    xTaskCreate( vWireTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_WIRE, NULL );
}

void vWireRequest(wireMessage* xMessage)
{
    xQueueSend( xWireQueue, xMessage, wireBLOCK_TIME );
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

    I2C_Send(WRITE_9DOF, I2CBUFF, 2);

//    i2c_write_byte(1,0,WRITE_9DOF);
//    i2c_write_byte(0,0,0x2D);
//    i2c_write_byte(0,1,0x08);

    vConsolePuts("9DOF Initialized.");
}

static void vTest9Dof()
{
    static char str[35];
    char I2CBUFF[2] = {0, 0};
    int x, y, z;

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
    I2C_Send(WRITE_9DOF, I2CBUFF, 1);
    I2C_Read(READ_9DOF, I2CBUFF, 2);
    x = I2CBUFF[0] | (I2CBUFF[1] << 8);

    I2CBUFF[0] = 0x34;
    I2C_Send(WRITE_9DOF, I2CBUFF, 1);
    I2C_Read(READ_9DOF, I2CBUFF, 2);
    y = I2CBUFF[0] | (I2CBUFF[1] << 8);

    I2CBUFF[0] = 0x36;
    I2C_Send(WRITE_9DOF, I2CBUFF, 1);
    I2C_Read(READ_9DOF, I2CBUFF, 2);
    z = I2CBUFF[0] | (I2CBUFF[1] << 8);

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
    i2c_write_byte(1,0,READ_RTC);

}

static void vSoftI2cScan()
{
    static unsigned char addr = 0x00;
//    char I2CBUFF[5]={0x00,0x0F,0xF0,0x01,0x10};
//    if( I2C_Send(addr,I2CBUFF,0) )
    if( !i2c_write_byte(1,1,addr) )
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
