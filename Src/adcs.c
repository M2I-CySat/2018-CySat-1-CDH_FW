#include "adcs.h"
#include "cmsis_os.h"
#include "uart2.h"
#include "heap.h"
#include "utilities.h"
#include "assert.h"



static inline int send_telecommand(uint8_t id, uint8_t * data);
static inline int request_telemetry(uint8_t id, uint8_t * data);

struct adcs_queue_item {
	int flags;
};

osMailQId adcs_queueHandle;
osMailQDef(adcs_queue, 4, struct adcs_queue_item); 

/* Initialize the ADCS mail queue */
int ADCS_Init()
{
	if ((adcs_queueHandle = osMailCreate(osMailQ(adcs_queue), NULL)) == NULL) {
		Debug_Printf("ERROR - Unable to create ADCS Queue");
		hard_failure();
	}

	return 0;
}

/* Process items in the mail queue */
void ADCSTask()
{
	for(;;){
		osEvent evt;
		evt = osMailGet(adcs_queueHandle, osWaitForever);
		if(evt.status == osEventMail){
			struct adcs_queue_item * item = evt.value.p;
			/* Handle item */
		}
	}
}

int ADCS_Send_Command(uint8_t command)
{
	
	return -1;
}

int ADCS_Get_Data(uint8_t data)
{
	
	return -1;
}

/* Send the start condition, then write address, then frame id, then start condition,
 * then read adress, then the data bytes can be red until the stop condition */
static inline int send_telecommand(uint8_t id, uint8_t * data)
{
	
	return -1;
}

/* Send start condition, write adress, id, then the data, then stop condition 
 * The person who calls this should do a sanity check to make sure the ADCS
 * is actually updated */
static inline int request_telemetry(uint8_t id, uint8_t * data)
{

	return -1;
}
