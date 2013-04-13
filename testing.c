
//#define TESTING

#ifdef TESTING

#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "pic24.h"

#include "uart.h"
#include "testing.h"

#define print(str)          vConsolePrint(str)
#define puts(str)           vConsolePuts(str)
#define putsError(str)      vConsolePutsError(str)

/* TO/DO: Need better way to do this (Don't judge me) */
#define COMMAND(str)        if((!sMatchCommand)&&(strcmp(pcBase,str)==0)&&((sMatchCommand = 1)))
#define ARG(str)            if(strcmp(pcArgs,str)==0)
#define HELP(str)           if(isFlagSet(pcArgs,"h",NULL)||isFlagSet(pcArgs,"-help",NULL)){puts(str);return;}
#define FLAG(str)           if(isFlagSet(pcArgs,str,NULL))
#define FLAGVAL(str,val)    if(isFlagSet(pcArgs,str,val))

BOOL isFlagSet( char* pcArgs, const char* flag, char* value )
{
    unsigned short i, j = 0;
    BOOL isFlag = FALSE;

    for( i=0; pcArgs[i]; ++i )
    {
        if( !isFlag && pcArgs[i] == '-' && pcArgs[i+1] )
        {
            isFlag = TRUE;
        }
        /* Check for matching substring */
        else if( isFlag && pcArgs[i] == flag[j] )
        {
            ++j;
            /* Check for match done */
            if( !flag[j] )
            {
                /* Find flag value */
                if( value != NULL && pcArgs[i] && pcArgs[i+1] )
                {
                    ++i;
                    for( ; pcArgs[i] && pcArgs[i] != ' '; ++i )
                    {
                        *value = pcArgs[i];
                        ++value;
                    }
                    *value = '\0';
                }
                return TRUE;
            }
        }
        else
        {
            isFlag = FALSE;
            j = 0;
        }
    }
    return FALSE;
}

void vStrip( char* pcArgs )
{
    BOOL isFlag = FALSE;
    char* pcNew = pcArgs;

    for( ; *pcArgs; ++pcArgs )
    {
        if( *pcArgs == '-' )
        {
            isFlag = TRUE;
        }
        else if ( *pcArgs == ' ' )
        {
            isFlag = FALSE;
        }

        if( !isFlag && *pcArgs != ' ' )
        {
            *pcNew = *pcArgs;
            ++pcNew;
        }
    }
    
    *pcNew = '\0';
}
#endif

void vInterpretTestingCommand( char* pcCommand )
{
#ifdef TESTING
    static char pcBase[uartBUFFER_SIZE];
    static char pcArgs[uartBUFFER_SIZE];

    short sMatchCommand = 0;
    short i;

    pcBase[0] = '\0';
    pcArgs[0] = '\0';

    for( i=0; i<uartBUFFER_SIZE; ++i )
    {
        pcBase[i] = pcCommand[i];

        if( pcCommand[i] == ' ' )
        {
            pcBase[i] = '\0';
            sprintf(pcArgs, "%s", &(pcCommand[i+1]));
            break;
        }

        if( pcCommand[i] == '\0' )
        {
            break;
        }
    }

    /* You are allowed to use 'return' inside COMMAND blocks */

    COMMAND( "" )
    {
        return;
    }

    COMMAND( "radio" )
    {
#define RADIO_HELP_STRING \
            "Usage: radio COMMAND\r\n" \
            "Sends a command to the Helium Radio board.\r\n" \
            "Commands:\r\n" \
            "  on\r\n" \
            "  off\r\n" \
            "  noop\r\n" \
            "  reset\r\n" \
            "  txmessage\r\n" \
            "  getconfig\r\n" \
            "  fastpa\r\n" \
            "  pattern\r\n" \
            "  txpattern\r\n" \
            "  telem\r\n" \
            "  beacon\r\n" \
            "  nmoson\r\n" \
            "  nmosoff\r\n" \

        HELP( RADIO_HELP_STRING );

        vStrip( pcArgs );

        ARG( "" )
        {
            puts(RADIO_HELP_STRING);
            return;
        }

        ARG( "noop" )
        {
            vRadioNoOP();
            return;
        }
        ARG( "getconfig" )
        {
            vRadioGetConfig();
            return;
        }
        ARG( "telem" )
        {
            vRadioTelemetry();
            return;
        }

        print( "Command '" );
        print( pcArgs );
        puts( "' is not implemented." );
    }

    COMMAND( "uartrelay" )
    {
        HELP(
                "Relays UART1 Rx -> UART2 Tx and UART2 Rx -> UART1 Tx.\r\n"
                "Board must be reset to undo."
            );
        
        vUartRelayMode( 1 );
    }

    COMMAND( "u1" )
    {
        vUart1Puts( pcArgs );
    }

    COMMAND( "u2" )
    {
        vUart2Puts( pcArgs );
    }

    COMMAND( "sleep" )
    {
        HELP( "Delays Console task by n milliseconds" );
        vTaskDelay( atoi( pcArgs ) );
    }

    COMMAND( "echo" )
    {
        puts( pcArgs );
    }

    COMMAND( "whoami" )
    {
        puts( "cysat" );
    }

    COMMAND( "help" )
    {
        puts("Use \"command -h\" for help with a specific command");
        puts("Useful commands");
        puts("UART: uartrelay, u1, u2, echo");
        puts("Radio: radio");
    }

    COMMAND( "ls" )
    {
        putsError( "Do I look like a UNIX machine?!" );
    }

    COMMAND( "rm" )
    {
        putsError( "Nice try, jerk." );
    }

    if( !sMatchCommand )
    {
        static char pcEchoCommand[uartBUFFER_SIZE + 40];
        sprintf( pcEchoCommand, "I don't understand '%s'", pcBase );
        putsError( pcEchoCommand );
    }
#endif
}
