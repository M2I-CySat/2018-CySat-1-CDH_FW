/* 
 * File:   uart.h
 * Author: Kris Scott
 *
 * Created on September 26, 2012, 6:40 PM
 */

#ifndef UART_H
#define	UART_H

#define BUF_SIZE 128
#define BACKSPACE 0x8

//#define BRATE       25              /* 9600 Bd (BREGH=0) */
//#define U_ENABLE    0x8000          /* enable UART, 1 stop, no parity */
//#define U_TX        0x0400          /* enable transmission, clear all flags */

#define uartBUFFER_SIZE      ( 80 + 1 )

/* Create the task that will control the UART.  Returned is a handle to the queue
on which messages to get written to the UART should be written. */
void vUartStartTask( void );

void vUart1Putc(char cChar);
void vUart1Puts(char* pcMessage);
void vUart2Putc(char cChar);
void vUart2Puts(char* pcMessage);
void vConsolePrint( char *pcString );
void vConsolePuts(char* pcMessage);
void vConsolePutx(char c);
void vConsolePutsError(char* pcMessage);

void vUartRelayMode( int mode );

#endif	/* UART_H */

