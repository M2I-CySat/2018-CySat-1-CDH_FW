/* 
 * File:   soft_i2c.h
 * Author: Kris
 *
 * Created on December 1, 2012, 12:32 PM
 */

#ifndef SOFT_I2C_H
#define	SOFT_I2C_H

char I2C_Send(char Address,char *Data,char Num);
char I2C_Read(char Address,char *Data,char Num);
char i2csendbyte(char byte);
char i2cgetbyte(void);
char i2cgetack(void);
void i2csendack(void);
void i2cdelay(char delay);
void i2cstart(void);
void i2cstop(void);
void i2cclock(void);
char i2creadbit(void);
//void pause(unsigned char sec);

#endif	/* SOFT_I2C_H */

