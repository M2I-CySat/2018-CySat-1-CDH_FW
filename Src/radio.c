#include "radio.h"
#include "sys_i2c.h"
#include <string.h>

#define RADIO_ADDRESS 0x25
#define PREAMBLE_ONE 0x1A
#define PREAMBLE_TWO 0xCF

int RADIO_Transmit(uint8_t * data, size_t len)
{
  int retval = 0;

  if (len > 256) {
    retval = -1;
  } else {
    uint8_t radio_protocol[len+4];

    radio_protocol[0] = PREAMBLE_ONE; /* Add on preamble as
      described in the radio tech sheets */
    radio_protocol[1] = PREAMBLE_TWO;
    radio_protocol[2] = len-1; /* 0=1, 1=2, etc. */
    memcpy(radio_protocol+3, data, len);

    uint8_t checksum = 0;
    uint8_t pos;
    for (pos = 0; pos < len; pos++) {
      checksum += data[pos]; /* Compute checksum */
    }
    radio_protocol[len+3] = checksum;

    if (SYS_I2C_Write(RADIO_ADDRESS, data, len)) {
      retval = -1;
    }
  }

  return retval;
}

uint8_t * RADIO_Receive()
{
  size_t len = 0; //TODO: Radio handles this, but how do I get it?
  uint8_t data[len];

  SYS_I2C_Read(RADIO_ADDRESS, data, len); /* Something else handles
    extracting data from this */
  return data;
}
