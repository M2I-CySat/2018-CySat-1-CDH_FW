#include "tasknames.h"
#include "cmsis_os.h"

extern osThreadId defaultTaskHandle;
extern osThreadId heartbeatTaskHandle;

const char * GetTaskName(void)
{
	osThreadId id = osThreadGetId();
	if (id == defaultTaskHandle) {
		return "Default";
	} else if (id == heartbeatTaskHandle) {
		return "Heartbeat";
	} else {
		return "?";
	}
}
