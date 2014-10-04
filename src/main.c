
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
#include "storage.h"
#include "system.h"
//#include "rtc.h"

//JTAG off, Code Protect off, Write Proect off, COE mode off, WDT off
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF )
//Clock switching/monitor off, Oscillator (RC15) on,
// Oscillator in HS mode, Use primary oscillator (no PLL)
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_ON & POSCMOD_HS & FNOSC_PRI )

/*Address Defines*/
#define FIRST_BOOT      0x000000
#define ANTENNA_STATUS  0x000001


/*Use actual init system. Disables all other defines*/
#define enInit              1

#if enInit
#define enUart              0
#define enConsoleCommand    0
#define enRadioCommand      0
#define enWire              0
#define enPowerTest         0
#define enUartTest          0
#define enPayload           0
#define enAutoBurn          0
#define enMem               0
#define enClock             0

#else
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
#endif

/*Init system config*/

#define BURN_DELAY          15 /*Seconds until antenna burn*/



static void initTask(void * params);
static void init();

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

#if enInit
    init();
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

static void initTask(void * params) {
    unsigned char buffer;
    vUartStartTask();
    vConsolePrintf("Init task started\r\n");

#if SET_FIRST_BOOT
    buffer = 0;
    vFRAMWrite(FIRST_BOOT, 1, &buffer);
    vConsolePrintf("Set First Boot Flag 0\r\n");
#endif
#if SET_NOT_FIRST_BOOT
    buffer = 1;
    vFRAMWrite(FIRST_BOOT, 1, &buffer);
    vConsolePrintf("Set First Boot 1\r\n");
#endif

    vFRAMRead(FIRST_BOOT, 1, &buffer);
    vConsolePrintf("Read first boot flag\r\n");
    if (!buffer) { /*This is first boot*/
        vConsolePrintf("First Boot - Doing Stuff\r\n");
        zeroRTC();
        vConsolePrintf("RTC Set Zero\r\n");
        buffer = 0;
        vFRAMWrite(ANTENNA_STATUS, 1, &buffer);
        vConsolePrintf("Antenna Status Set Zero\r\n");
        buffer = 1;
        vFRAMWrite(FIRST_BOOT, 1, &buffer);
        vConsolePrintf("First Boot Set 1\r\n");
    }

    vConsolePrintf("Past first boot check\r\n");

    vStartClockTask();
    vTaskDelay(1500);

    char timeBuffer[15];
    sprintf(timeBuffer, "Time: %ld\r\n", getMissionTime());
    vConsolePrintf(timeBuffer);
    vConsolePrintf("Clock Started - Waiting for Burn\r\n");

    while(getMissionTime() <= BURN_DELAY) {
        vTaskDelay(500);
    }
    vConsolePrintf("Burn Delay Reached - Checking Status\r\n");
    vFRAMRead(ANTENNA_STATUS, 1, &buffer);
    if (!buffer) {
        vConsolePrintf("Burning\r\n");
        vNichromeTask(0);
    }
    vConsolePrintf("Burn Complete - Setting Deploy Status\r\n");
    buffer = 1;
    vFRAMWrite(ANTENNA_STATUS, 1, &buffer);
    vConsolePrintf("Deploy Status Set\r\n");

    vConsolePrintf("Starting Command Handling\r\n");
    xStartUart1CommandHandling();

    startStorageDriverTask();

    while(1){
        vTaskDelay(10000);
    }
}
static void init() {
    vSetupMem();
    vWireInit();
    xTaskCreate( initTask, NULL, configMINIMAL_STACK_SIZE + 50, NULL, systemPRIORITY_INIT, NULL );
}


void vApplicationIdleHook( void )
{
	/* Schedule the co-routines from within the idle task hook. */
//	vCoRoutineSchedule();
}

