#include "uart2.h"
#include <stdint.h>
#include "cmsis_os.h"
#include "mutexes.h"

void HeartbeatTask(const void * arguments)
{
	UART2_Printf("Heartbeat task started!\r\n");
	for(;;) {
		osDelay(2000);
		UART2_Printf("HB\r\n");
	}
}
