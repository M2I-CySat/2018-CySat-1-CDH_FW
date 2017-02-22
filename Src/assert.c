/* Definitions for various epic fail handlers */

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
