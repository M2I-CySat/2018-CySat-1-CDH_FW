#include "tasknames.h"
#include "cmsis_os.h"

extern osThreadId defaultTaskHandle;
extern osThreadId heartbeatTaskHandle;
extern osThreadId radio_TxTaskHandle;

const char * GetTaskName(void)
{
	osThreadId id = osThreadGetId();
	if (id == defaultTaskHandle) {
		return "Default";
	} else if (id == heartbeatTaskHandle) {
		return "Heartbeat";
	} else if (id == radio_TxTaskHandle) {
		return "Radio TX";
	} else {
		return "?";
	}
}
