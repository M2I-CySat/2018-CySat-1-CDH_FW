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

	Test_Mem();
	//Test_Radio();

	for(;;) {
		osDelay(2000);
		Debug_Printf("Default");
	}
}
