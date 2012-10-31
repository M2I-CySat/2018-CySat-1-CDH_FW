

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
    while (!IFS3bits.MI2C2IF); //Wait for ninth clock cycle
    while (I2C2STATbits.ACKSTAT);
}

BYTE ucWireRead()
{
    BYTE r = MasterReadI2C2();
    while (!IFS3bits.MI2C2IF); //Wait for ninth clock cycle
    AckI2C2();
    return r;
}

BYTE ucWireReadAndStop()
{
    BYTE r = MasterReadI2C2();
    while (!IFS3bits.MI2C2IF); //Wait for ninth clock cycle
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

static void vWireTask( void *pvParameters )
{
    wireMessage xMessage;

    for( ;; )
    {
        if( xQueueReceive( xWireQueue, &xMessage, wireBLOCK_TIME ) )
        {
            xMessage.callback();
        }
    }
}
