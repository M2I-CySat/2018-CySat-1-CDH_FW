/* Definitions for various epic fail handlers */

#include "main.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "uart2.h"

void hard_failure()
{
	for(;;);
}

void vApplicationMallocFailedHook()
{
	Debug_Printf("!!!!!!!!!! MALLOC FAILED !!!!!!!!!!");
	for(;;);
}

void vApplicationStackOverflowHook(
		xTaskHandle xTask,
		signed char * pcTaskName)
{
	for(;;);
}
