/* Definitions for various epic fail handlers */

#include "main.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"

void hard_failure()
{
	for(;;);
}

void vApplicationMallocFailedHook()
{
	for(;;);
}

void vApplicationStackOverflowHook(
		xTaskHandle xTask,
		signed char * pcTaskName)
{
	for(;;);
}
