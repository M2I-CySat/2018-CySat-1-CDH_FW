#include "FreeRTOS.h"
#include "i2c_wiki.h"

/* The SCL output pin */
#define SCL         _RG2
/* The SCL Direction Register Bit */
#define SCL_TRIS    _TRISG2
/* The SDA output pin */
#define SDA         _RG3
/* The SDA Direction Register Bit */
#define SDA_TRIS    _TRISG3

// Hardware-specific support functions that MUST be customized:
#define I2CSPEED  100
void I2C_delay() { 
    volatile int v; 
    int i;
    for (i=0; i < I2CSPEED/2; i++) v;
}
// Set SCL as input and return current level of line, 0 or 1
BOOL read_SCL(void)
{
    SCL_TRIS = 1;
    return (SCL != 0);
}
// Set SDA as input and return current level of line, 0 or 1
BOOL read_SDA(void)
{
    SDA_TRIS = 1;
    return (SDA != 0);
}
// Actively drive SCL signal low
void clear_SCL(void)
{
    SCL_TRIS = 0;
}
// Actively drive SDA signal low
void clear_SDA(void)
{
    SDA_TRIS = 0;
}

void arbitration_lost(void)
{

}

BOOL started = FALSE; // global data
void i2c_start_cond(void) {
  if (started) { // if started, do a restart cond
    // set SDA to 1
    read_SDA();
    I2C_delay();
    while (read_SCL() == 0) {   // Clock stretching
      // You should add timeout to this loop
    }
    // Repeated start setup time, minimum 4.7us
    I2C_delay();
  }
  if (read_SDA() == 0) {
    arbitration_lost();
  }
  // SCL is high, set SDA from 1 to 0.
  clear_SDA();
  I2C_delay();
  clear_SCL();
  started = TRUE;
}

void i2c_stop_cond(void){
  // set SDA to 0
  clear_SDA();
  I2C_delay();
  // Clock stretching
  while (read_SCL() == 0) {
    // You should add timeout to this loop.
  }
  // Stop bit setup time, minimum 4us
  I2C_delay();
  // SCL is high, set SDA from 0 to 1
  if (read_SDA() == 0) {
    arbitration_lost();
  }
  I2C_delay();
  started = FALSE;
}

// Write a bit to I2C bus
void i2c_write_bit(BOOL b) {
  if (b) {
    read_SDA();
  } else {
    clear_SDA();
  }
  I2C_delay();
  while (read_SCL() == 0) {   // Clock stretching
    // You should add timeout to this loop
  }
  // SCL is high, now data is valid
  // If SDA is high, check that nobody else is driving SDA
  if (b && read_SDA() == 0) {
    arbitration_lost();
  }
  I2C_delay();
  clear_SCL();
}

// Read a bit from I2C bus
BOOL i2c_read_bit(void) {
  BOOL b;
  // Let the slave drive data
  read_SDA();
  I2C_delay();
  while (read_SCL() == 0) {   // Clock stretching
    // You should add timeout to this loop
  }
  // SCL is high, now data is valid
  b = read_SDA();
  I2C_delay();
  clear_SCL();
  return b;
}

// Write a byte to I2C bus. Return 0 if ack by the slave.
BOOL i2c_write_byte(BOOL send_start,
                    BOOL send_stop,
                    unsigned char byte) {
  unsigned b;
  BOOL nack;
  if (send_start) {
    i2c_start_cond();
  }
  for (b = 0; b < 8; b++) {
    i2c_write_bit((byte & 0x80) != 0);
    byte <<= 1;
  }
  nack = i2c_read_bit();
  if (send_stop) {
    i2c_stop_cond();
  }
  return nack;
}

// Read a byte from I2C bus
unsigned char i2c_read_byte(BOOL nack, BOOL send_stop) {
  unsigned char byte = 0;
  unsigned b;
  for (b = 0; b < 8; b++) {
    byte = (byte << 1) | i2c_read_bit();
  }
  i2c_write_bit(nack);
  if (send_stop) {
    i2c_stop_cond();
  }
  return byte;
}
