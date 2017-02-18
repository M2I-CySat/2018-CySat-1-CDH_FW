#include "uart2.h"
#include <stdint.h>
#include "cmsis_os.h"

void HeartbeatTask(const void * arguments)
{
	osDelay(15000);
	UART2_Printf("Hartbeat task started!\r\n", 69);
	uint32_t hb = 0;
	for(;;) {
		UART2_Printf("HB: %d\r\n", hb);
		osDelay(1500);
	}
}
