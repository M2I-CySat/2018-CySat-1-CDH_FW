/*!
 * \File   nichrome.h
 * Author: kscott
 *
 * Created on April 27, 2013, 1:36 PM
 * 
 * \addtogroup CySat
 * \{
 * \addtogroup CySat_Private
 * \{
 */

#ifndef NICHROME_H
#define	NICHROME_H

/**
 * Starts the Nichrome wire burning task
 */
void vNichromeStartTask();

/**
 * RTOS task to control nichrome burn.
 * @param pvParameters
 */
void vNichromeTask( void *pvParameters );

#endif	/* NICHROME_H */

/*!
 * \}
 * \}
 */