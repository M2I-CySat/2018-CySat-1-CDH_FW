
/* Standard includes. */

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "mem.h"

/* Application includes */
#include "serial.h"
#include "uart.h"
#include "wire.h"

#include "power.h"
#include "payload.h"
#include "comms.h"
#include "nichrome.h"
//#include "rtc.h"

//JTAG off, Code Protect off, Write Proect off, COE mode off, WDT off
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF )
//Clock switching/monitor off, Oscillator (RC15) on,
// Oscillator in HS mode, Use primary oscillator (no PLL)
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_ON & POSCMOD_HS & FNOSC_PRI )

int main( void )
{
    /* Enable UART */
    vUartStartTask();

    /* Enable Wire (I2C) */
    vWireInit();
//    vWireStartTask(); // Don't use anymore
    
//    vPowerStartTask();
//    vPayloadStartTask();
//    vCommsStartTask();
//    vNichromeStartTask();

    //vUartStartTestTask();

    //vConsolePrint( "\r\nI am a UART!\r\n" );
    //vUart1Print("(Uart1)\r\n");
//    vUart2Print("(Uart2)\r\n");

    vStartMemTestTask();


    /* Finally start the scheduler. */
    vTaskStartScheduler();

    /* Will only reach here if there is insufficient heap available to start
    the scheduler. */
    return 0;
}

void vApplicationIdleHook( void )
{
	/* Schedule the co-routines from within the idle task hook. */
//	vCoRoutineSchedule();
}

