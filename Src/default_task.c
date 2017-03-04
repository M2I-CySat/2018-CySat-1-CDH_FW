#include "uart2.h"
#include "cmsis_os.h"
#include "mutexes.h"
#include "gpio.h"

void DefaultTask(const void * arguments)
{
	Debug_Printf("Default task started");
	for(;;) {
		osDelay(2000);
		Debug_Printf("Default");
	}
}
