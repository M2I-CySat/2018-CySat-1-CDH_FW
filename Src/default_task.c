#include "default_task.h"

#include "uart2.h"

void DefaultTask()
{
	UART2_Write("Hello, world!\r\n", 15);
	for(;;);
}
