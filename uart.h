/* 
 * File:   uart.h
 * Author: Kris Scott
 *
 * Created on September 26, 2012, 6:40 PM
 */

#ifndef UART_H
#define	UART_H

#include "serial.h"

#define BACKSPACE 0x8

//#define BRATE       25              /* 9600 Bd (BREGH=0) */
//#define U_ENABLE    0x8000          /* enable UART, 1 stop, no parity */
//#define U_TX        0x0400          /* enable transmission, clear all flags */

#define uartRX_BUFFER_SIZE      80
#define uartSPRINTF_BUFFER_SIZE 256

#ifdef serialALTERNATE_IMPLEMENTATION
extern xComPortHandle xUart1Handle;
extern xComPortHandle xUart2Handle;
#endif

/**
 * Create the task that will control the UART.
 */
void vUartStartTask( void );

/**
 * Transmit a byte to UART 1
 * @param c byte of data
 */
void vUart1Put( char c );

/**
 * Transmit a string to UART 1
 * @param s null-terminalted string
 */
void vUart1Print( char *s );

/**
 * Do a printf directly to UART 1
 * @param fmt (See printf)
 * @param ... (See printf)
 */
void vUart1Printf( const char *fmt, ... );

/**
 * Transmit a byte to UART 2
 * @param c byte of data
 */
void vUart2Put( char c );

/**
 * Transmit a string to UART 2
 * @param s null-terminalted string
 */
void vUart2Print( char *s );

/**
 * Do a printf directly to UART 2
 * @param fmt (See printf)
 * @param ... (See printf)
 */
void vUart2Printf( const char *fmt, ... );

/**
 * Transmit a byte to Console
 * @param c byte of data
 */
void vConsolePut( char c );

/**
 * Transmit a string to Console
 * @param s null-terminalted string
 */
void vConsolePrint( char *s );

/**
 * Do a printf directly to Console
 * @param fmt (See printf)
 * @param ... (See printf)
 */
void vConsolePrintf( const char *fmt, ... );

/**
 * Do a printf, with an error message, directly to Console
 * @param fmt (See printf)
 * @param ... (See printf)
 */
void vConsoleErrorPrintf( const char *fmt, ... );


#endif	/* UART_H */

