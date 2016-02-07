/* Error handlers */

#ifndef __ERROR_H
#define __ERROR_H

/* RTOS-Related errors */
void ERROR_ResourceFrozen(char * msg);
void ERROR_MiscRTOS(char * msg);

/* Hardware Errors */
void ERROR_Uninitialized(char * msg);
void ERROR_NotImplemented(char * msg);

#endif