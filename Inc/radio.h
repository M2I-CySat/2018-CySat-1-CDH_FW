#ifndef _radio_h
#define _radio_h

#include <stdint.h>
#include <stdlib.h>
#include "cmsis_os.h"
/* Public functions for radio */

/* Initialization, call at satellite powerup */
int Radio_Init(void);

/* Post a message to the radio transmit queue */
int Radio_Transmit(uint8_t * data, size_t len);

#define MAX_DATA_SIZE 255
struct tx_queue_item {
	size_t len;
	uint8_t data[MAX_DATA_SIZE];
};

extern osMailQId tx_queueHandle;


#endif
