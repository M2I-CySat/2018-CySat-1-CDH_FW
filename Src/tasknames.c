#include "tasknames.h"
#include "cmsis_os.h"

extern osThreadId defaultTaskHandle;
extern osThreadId heartbeatTaskHandle;
extern osThreadId radio_TxTaskHandle;
extern osThreadId radio_RxTaskHandle;
extern osThreadId heapTaskHandle;
extern osThreadId epsTaskHandle;
extern osThreadId downlinkTaskHandle;

const char * GetTaskName(void)
{
	osThreadId id = osThreadGetId();
	if (id == defaultTaskHandle) {
		return "Default";
	} else if (id == heartbeatTaskHandle) {
		return "Heartbeat";
	} else if (id == radio_TxTaskHandle) {
		return "Radio TX";
	} else if (id == radio_RxTaskHandle) {
		return "Radio RX";
	} else if (id == epsTaskHandle) {
		return "EPS";
	} else if (id == heapTaskHandle) {
		return "Heap";
	} else if (id == downlinkTaskHandle) {
		return "Downlink";
	} else {
		return "?";
	}
}
