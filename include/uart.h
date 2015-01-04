/*!
 * \File   uart.h
 * \Author Kris Scott
 * 
 * \addtogroup CySat
 * \{
 * \defgroup CySat_UART
 * \{
 *
 * \brief UART API wrapping stdio.h printf functions
 * 
 * Created on September 26, 2012, 6:40 PM
 * 
 * Use these functions for printf-style functionality.
 * 
 * Use vConsolePrintf() as the generic printf. Avoid 
 * explicitly using uart1 and uart2 unless there is a very
 * good reason.
 */

#ifndef UART_H
#define	UART_H

#include "serial.h"

// Define if you want to use test functions. e.g. vUartTest()
#define uartTEST_FUNCTIONS

/*! Maximum size of printf. Do not exceed
 */
#define uartSPRINTF_BUFFER_SIZE 256

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

/*!
 * \}
 * \}
 */

#endif	/* UART_H */

