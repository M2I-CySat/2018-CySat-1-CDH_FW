#ifndef _radio_h
#define _radio_h

#include <stdint.h>
#include <stdlib.h>

/* Public functions for radio */

/* Post a message to the radio transmit queue */
int RADIO_Transmit(uint8_t * data, size_t len);

#endif
