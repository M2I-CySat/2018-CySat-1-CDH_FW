/* 
 * File:   myprintf.h
 * Author: Kristoffer Scott
 *
 * Created on April 13, 2013, 12:22 PM
 *
 * This is my own, minimalistic, implementation of any *printf functions I need
 * in this project.  The standard C lib *printf uses a lot of space on the
 * stack, and crashes when the PIC is using FreeRTOS.  This is not meant to
 * provide a full implementation of *printf, only offer a few necessary
 * features.
 *
 * A format specifier follows this prototype:
 * %[flags][width]specifier
 *
 * A unsupported or malformed specifier will print "%?"
 *
 * Currently supported *printf features:
 * Specifier:
 *   %d  int
 *   %u  unsigned int
 *   %c  character
 *   %s  string (null-terminated)
 *   %%  literal '%'
 *
 * Flags (must be in this order if used):
 *   +   Add '+' to positive numbers
 *       (space) Add space in front of positive numbers (incompatable with '+')
 *   0   pad with 0
 *
 * Width:
 *   N (A number greater than 0) minimum width of this specifier
 *
 * Note: If you need rediculously large widths, make sure INT_BUF_SIZE is big
 * enough, and make it bigger if needed.
 * 
 */

#ifndef MYPRINTF_H
#define	MYPRINTF_H

#include <stdarg.h>
#include <stddef.h>

void myvsnprintf( char out[], size_t size, const char *fmt, va_list ap );
void mysnprintf( char *str, size_t size, const char *fmt, ... );

#endif	/* MYPRINTF_H */

