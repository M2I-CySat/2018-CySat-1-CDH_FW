#include "uart2.h"
#include <stdint.h>
#include "cmsis_os.h"
#include "mutexes.h"

void HeartbeatTask(const void * arguments)
{
	Debug_Printf("Heartbeat task started!");
	for(;;) {
		osDelay(2000);
		Debug_Printf("Heartbeat");
	}
}
