#include "uart2.h"
#include "cmsis_os.h"

void DefaultTask(const void * arguments)
{
	UART2_Write(".", 1);
	osDelay(1000);
	UART2_Write(".", 1);
	osDelay(1000);
	UART2_Write(".", 1);
	osDelay(1000);
	UART2_Write(".", 1);
	osDelay(1000);
	UART2_Printf("Hello, ");
	osDelay(1000);
	UART2_Printf("World!\r\n");
	UART2_Printf("Hello from Printf %d\r\n", 69);
	for(;;);
}
