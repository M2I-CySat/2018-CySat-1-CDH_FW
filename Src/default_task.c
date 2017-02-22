#include "uart2.h"
#include "cmsis_os.h"
#include "mutexes.h"

void DefaultTask(const void * arguments)
{
	UART2_Printf("Default task started\r\n");
	for(;;) {
		osDelay(2000);
		UART2_Printf("Default\r\n");
	}
}
