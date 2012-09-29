/* 
 * File:   pic24.h
 * Author: Kris
 *
 * Created on September 26, 2012, 6:35 PM
 */

#ifndef PIC24_H
#define	PIC24_H

#define FCY 4000000UL

#define COMPILE_TIME "Compiled " __DATE__ " " __TIME__

#define ISRNP void __attribute__((interrupt, no_auto_psv))

#include <stdio.h>
#include <stdlib.h>

#include <p24Fxxxx.h>
#include <p24fj128ga010.h>
#include <libpic30.h>

#include <GenericTypeDefs.h>

#endif	/* PIC24_H */

