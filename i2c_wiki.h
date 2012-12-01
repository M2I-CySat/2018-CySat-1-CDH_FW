/* 
 * File:   i2c_wiki.h
 * Author: Kris
 *
 * Created on December 1, 2012, 4:17 PM
 */

#ifndef I2C_WIKI_H
#define	I2C_WIKI_H

#include <GenericTypeDefs.h>

BOOL i2c_write_byte(BOOL send_start,BOOL send_stop,unsigned char byte);
unsigned char i2c_read_byte(BOOL nack, BOOL send_stop);

#endif	/* I2C_WIKI_H */

