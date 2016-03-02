/* CySat printf and friends */

#include <stdio.h>
#include <string.h>

#include <printf.h>
#include <error.h>

/* The static printf() buffer */

static char printf_buffer[PRINTF_BUFFER_SIZE];

#define MAX_THREADS 16
static const char * thread_names[MAX_THREADS];
static osThreadId thread_ids[MAX_THREADS];
static size_t num_threads_defined = 0;

static osMutexId buffer_lock_id;
osMutexDef(buffer_lock);

size_t dbg_add_thread(osThreadId id, const char * name)
{
	if (num_threads_defined < MAX_THREADS) {
		thread_ids[num_threads_defined] = id;
		thread_names[num_threads_defined] = name;
		return ++num_threads_defined;
	} else {
		return -1;
	}
}

const char * dbg_thread_name(osThreadId id)
{
	for(size_t i = 0; i < num_threads_defined; i++) {
		if (thread_ids[i] == id) {
			return thread_names[i];
		}
	}
	return NULL;
}

int PRINTF_Initialize()
{
	buffer_lock_id = osMutexCreate(osMutex(buffer_lock));
	if (buffer_lock_id == NULL) {
		ERROR_MiscRTOS("Failed to create printf mutex");
		return -1;
	}
	return 0;
}

int uprintf(enum UART_Uart uart, const char *format_string, ...)
{
	va_list args;
	va_start(args, format_string);

	/* Format into the buffer */
	int retval;
	retval = vuprintf(uart, format_string, args);

	va_end(args);

	return retval;
}

int dbg_printf(const char *format_string, ...)
{
	enum UART_Uart dbgUart;
	dbgUart = UART_GetDebug();

	/* Print severity and thread information */
	const char * name;
	osThreadId id;
	id = osThreadGetId();
	name = dbg_thread_name(id);
	if (name != NULL) {
		uprintf(dbgUart, "[DBG:%s] ", name);
	} else {
		uprintf(dbgUart, "[DBG:%d] ", id);
	}

	va_list args;
	va_start(args, format_string);

	int retval;
	retval = vuprintf(dbgUart, format_string, args);

	va_end(args);

	uputs("\r\n", dbgUart);


	return retval;
}

int vuprintf(enum UART_Uart uart, const char *format_string, va_list args)
{
	if (osMutexWait(buffer_lock_id, osWaitForever) != osOK){
		ERROR_ResourceFrozen("Unable to obtain printf buffer mutex");
	}

	/* Format into the buffer */
	vsnprintf(printf_buffer, PRINTF_BUFFER_SIZE, format_string, args);

	/* Print the buffer */
	int retval;
	retval = uputs(printf_buffer, uart);

	if (osMutexRelease(buffer_lock_id) != osOK){
		ERROR_MiscRTOS("Unable to release printf buffer mutex");
	}
	return retval;
}

int uputs(const char * s, enum UART_Uart uart)
{	
	return UART_Write(uart, (uint8_t *)s, strlen(s));
}
