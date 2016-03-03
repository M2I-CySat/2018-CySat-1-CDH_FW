#include <error.h>
#include <drivers/nichrome.h>
#include <cmsis_os.h>
#include <printf.h>

static void antenna_thread(void const *params);
osThreadDef(antenna_thread, osPriorityNormal, 1, 0);
static osThreadId antenna_thread_id;

int ANTENNAS_Deploy()
{
	dbg_printf("Starting antenna thread");
	antenna_thread_id = osThreadCreate(osThread(antenna_thread), NULL);
	
	if (antenna_thread_id == NULL) {
		ERROR_MiscRTOS("No thread was created");
		return -1;
	} else {
		return 0;
	}
}

static void antenna_thread(void const *params)
{
	dbg_add_thread(osThreadGetId(), "antenna");
	dbg_printf("Thread started");
	NICHROME_On(NICHROME_Burner_1);
	osDelay(5000);
	NICHROME_Off(NICHROME_Burner_1);
	NICHROME_On(NICHROME_Burner_2);
	osDelay(5000);
	NICHROME_Off(NICHROME_Burner_2);
}
