
#include "nichrome.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define nichromeTRIS1 _TRISF0
#define nichromeTRIS2 _TRISG0
#define nichromeTRIS3 _TRISA15
#define nichromeTRIS4 _TRISB0
#define nichromeTRIS5 _TRISB7
#define nichromeTRIS6 _TRISB9

#define nichromeCTRL1 _RF0
#define nichromeCTRL2 _RG0
#define nichromeCTRL3 _RA15
#define nichromeCTRL4 _RB0
#define nichromeCTRL5 _RB7
#define nichromeCTRL6 _RB9

#define nichromeON  1
#define nichromeOFF 0

#define BURN_TIME 4000

//number of seconds before burn
//used for an automated burn after deployment
#define BURN_INITIAL_DELAY 0

/* Print messages to Console? */
#define nichromeLOCAL_PRINT

#ifdef nichromeLOCAL_PRINT
#include "uart.h"
#endif

/**
 * Turn on Nichrome Burn Enable for specified amount of time
 * @param ctrl Which nichrome wire to burn (1-6)
 * @param ms Time in milliseconds to burn
 */
static void vNichromeBurn(char ctrl, unsigned ms);

/**
 * Initialize all nichrome control pins to OFF
 */
static void vNichromeInit();


/**
 * Set a nichrome control
 * @param ctrl
 * @param val
 */
inline static void vNichromeSet(char ctrl, unsigned val);

/**
 * Set a nichrome control TRIS
 * @param ctrl
 * @param val
 */
inline static void vNichromeTrisSet(char ctrl, unsigned val);


/**
 * Print an error for a bad control
 * @param ctrl
 */
inline static void vBadCtrlError( char ctrl );


/* Implementation*/


inline static void vBadCtrlError( char ctrl )
{
#ifdef nichromeLOCAL_PRINT
    vConsoleErrorPrintf( "Bad Nichrome Control (expected 1-6, got %d)\r\n", ctrl );
#endif
}

inline static void vNichromeSet(char ctrl, unsigned val)
{
    switch (ctrl)
    {
        case 1: nichromeCTRL1 = val; return;
        case 2: nichromeCTRL2 = val; return;
        case 3: nichromeCTRL3 = val; return;
        case 4: nichromeCTRL4 = val; return;
        case 5: nichromeCTRL5 = val; return;
        case 6: nichromeCTRL6 = val; return;
    }
    vBadCtrlError(ctrl);
}

inline static void vNichromeTrisSet(char ctrl, unsigned val)
{
    switch (ctrl)
    {
        case 1: nichromeTRIS1 = val; return;
        case 2: nichromeTRIS2 = val; return;
        case 3: nichromeTRIS3 = val; return;
        case 4: nichromeTRIS4 = val; return;
        case 5: nichromeTRIS5 = val; return;
        case 6: nichromeTRIS6 = val; return;
    }
    vBadCtrlError(ctrl);
}

static void vNichromeInit()
{
    /* I know this is a thinly-veiled for-switch pattern... */
    char ctrl;
    for (ctrl = 1; ctrl <= 6; ++ctrl)
    {
        vNichromeTrisSet(ctrl, 0); /* Set TRIS to output */
        vNichromeSet(ctrl, nichromeOFF); /* Turn off */
    }
}

static void vNichromeBurn(char ctrl, unsigned ms)
{
#ifdef nichromeCTRL1
#ifdef nichromeLOCAL_PRINT
    vConsolePrintf("Burn #%d (%d ms) start...", ctrl, ms);
#endif
    vNichromeSet(ctrl, nichromeON);
    vTaskDelay(ms);
    vNichromeSet(ctrl, nichromeOFF);
#ifdef nichromeLOCAL_PRINT
    vConsolePrint("Done.\r\n");
#endif
#endif /* ifdef nichromeCTRL1 */
}

void vNichromeStartTask()
{
    vNichromeInit();
    xTaskCreate( vNichromeTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_NICHROME, NULL );
}

void vNichromeTask( void *pvParameters )
{
    char ctrl;
    vConsolePrint("Delayed burn initialized...\r\n");

    int i = 0;
    for(i = 0; i < BURN_INITIAL_DELAY; i++) {
        vTaskDelay(1000);
    }

#ifdef nichromeLOCAL_PRINT
    vTaskDelay(1000);
    vConsolePrint("\r\nBurn in 3...\r");
    vTaskDelay(1000);
    vConsolePrint("Burn in 2...\r");
    vTaskDelay(1000);
    vConsolePrint("Burn in 1...\r");
    vTaskDelay(1000);
#else
    vTaskDelay(4000);
#endif

    /* Burn all 6 wires, 1 at a time */
    for (ctrl = 1; ctrl <= 6; ++ctrl)
    {
        vNichromeBurn(ctrl, BURN_TIME);
    }

    /* End task */
#if INCLUDE_vTaskDelete == 1
    vTaskDelete(NULL);
#else
#if 0
    for( ;; )
    {
        vTaskDelay(10000);
    }
#endif
#endif
}
