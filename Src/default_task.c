#include "uart2.h"
#include "cmsis_os.h"
#include "mutexes.h"
#include "gpio.h"
#include "tests.h"
#include "radio.h"

void DefaultTask(const void * arguments)
{
	Debug_Printf("Default task started");
	Debug_Printf("Running power-on tests");

	osDelay(500);
	Debug_Printf("Beginning tests...");
	osDelay(100);
	Debug_Printf("Testing mem");
	Test_Mem();

	osDelay(1000);
	Debug_Printf("Testing radio transmit");
	Test_RadioTransmit();

	for(;;) {
		osDelay(2000);
		Debug_Printf("Default");
	}
}
