
/* Standard includes. */
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"

/* Application includes */
#include "serial.h"
#include "uart.h"
#include "heliumradio.h"

//JTAG off, Code Protect off, Write Proect off, COE mode off, WDT off
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF )
//Clock switching/monitor off, Oscillator (RC15) on,
// Oscillator in HS mode, Use primary oscillator (no PLL)
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_ON & POSCMOD_HS & FNOSC_PRI )

int main( void )
{
    vRadioInit();

    vStartUartTask();

    /* Don't send UART Rx to radio by default */
    vUartRelayMode( 0 );

    vConsolePuts( "I am a UART!" );
    vUart1Puts("Uart1\r\n");
//    vUart2Puts("Uart2");

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
/*-----------------------------------------------------------*/

