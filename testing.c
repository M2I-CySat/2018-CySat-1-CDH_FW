#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "pic24.h"

#include "uart.h"
#include "testing.h"

/* TODO: Need better way to do this (Don't judge me) */
#define DEFINE_COMMANDS     if (0) {}
#define COMMAND(str)        else if (strcmp(base, str)==0)
#define DEFAULT_COMMAND     else

void interpretTestingCommand( char* command )
{
    static char pcEchoCommand[uartBUFFER_SIZE + 40];
    static char base[uartBUFFER_SIZE];

    short i;

    for( i=0; i<uartBUFFER_SIZE; ++i )
    {
        base[i] = command[i];

        if (base[i] == '\0' || base[i] == ' ')
        {
            base[i] = '\0';
            break;
        }
    }

    DEFINE_COMMANDS
    COMMAND( "whoami" )
    {
        vUartPuts( "cysat" );
    }
    COMMAND( "ls" )
    {
        vUartPutsError( "Do I look like a UNIX machine?!" );
    }
    COMMAND( "rm" )
    {
        vUartPuts("Nice try, jerk.");
    }
    DEFAULT_COMMAND
    {
        sprintf( pcEchoCommand, "I don't understand '%s'", base );
        vUartPutsError( pcEchoCommand );
    }
    
    vUartPuts("\r\n");
}
