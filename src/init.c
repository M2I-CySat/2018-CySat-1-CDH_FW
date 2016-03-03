/* init.c
 * 
 * CySat Primary MCU init application.
 *
 * Performs satellite boot-up functions
 */
 
#include <printf.h>
#include <cmsis_os.h>
#include <antennas.h>
 
void INIT_Init()
{
	dbg_printf("Performing PRI MCU initialization.");
	ANTENNAS_Deploy();
}
