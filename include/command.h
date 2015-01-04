/*!
 * \file command.h
 *
 *  Created on: Dec 28, 2013
 *  \Author jake
 * 
 * \addtogroup CySat
 * \{
 * \addtogroup CySat_Private
 * \{
 */

#ifndef COMMAND_H_
#define COMMAND_H_


/*
 * Start command handling on console UART
 */
void xStartUart1CommandHandling();

/*
 * Start command handling on radio UART
 * NOTE: This does not do really basically any validation or stuff on radio headers
 */
void xStartUart2CommandHandling();

/*!
 * \}
 * \}
 */

#endif /* COMMAND_H_ */
