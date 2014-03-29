
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
#include "command.h"
#include "clock.h"
//#include "rtc.h"

//JTAG off, Code Protect off, Write Proect off, COE mode off, WDT off
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF )
//Clock switching/monitor off, Oscillator (RC15) on,
// Oscillator in HS mode, Use primary oscillator (no PLL)
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_ON & POSCMOD_HS & FNOSC_PRI )


//feature defines. 0 to disable
#define enUart              1
#define enConsoleCommand    1
#define enRadioCommand      0
#define enWire              1
#define enPowerTest         0
#define enUartTest          0
#define enPayload           0
#define enAutoBurn          0
#define enMem               1
#define enClock             1

int main( void )
{

#if enUart
    vUartStartTask();
#endif

#if enWire
    vWireInit();
#endif

#if enConsoleCommand
    xStartUart1CommandHandling();
#endif

#if enRadioCommand
    xStartUart2CommandHandling();
#endif

#if enPowerTest
    vPowerStartTask();
#endif

#if enPayload
    vPayloadStartTask();
#endif

#if enPayload
    vUartStartTestTask();
#endif

#if enAutoBurn
    vNichromeStartTask();
#endif

#if enMem
    vSetupMem();
#endif

#if enClock
    vStartClockTask();
#endif

    /* No idea about this one*/
//    vCommsStartTask();

   // vSetupMem();
   // vStartMemTestTask();

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

