/* 
 * File:   data.h
 * Author: Jake Drahos
 *
 * Created on September 20, 2014, 1:22 PM
 *
 * Contains abstract data storage routines. This includes the data heap
 * as well as config storage. Once the data storage task is started,
 * it is illegal to directly call the mem read/write functions from any other
 * task.
 */

#ifndef DATA_H
#define	DATA_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <power.h>
    
    typedef enum {
        PUSH_HEAP,          //push OPERAND to heap, LENGTH is unit size
        POP_HEAP,           //pop from heap to OPERAND, LENGTH is unit size
        WRITE_CONFIG,       //copy LENGTH from OPERAND to OFFSET
        READ_CONFIG,        //copy LENGTH from OFFSET to OPERAND
        CLEAR_HEAP,         //Clear the heap
    } dataDriverCommand;

    typedef struct {
        dataDriverCommand command;
        unsigned char * operand;
        unsigned int offset;
        unsigned int length;
        unsigned char * completed; /*Flag set upon completion. Used to signal a completed read*/
    };

    void packHousekeeping(powerData *, unsigned char *);
    void unpackHousekeeping(unsigned char *, powerData *);

    /*queue up a command*/
    void sendDataDriverCommand(dataDriverCommand, unsigned char *, unsigned int, unsigned int, unsigned char * completed = 0);

    void startDataDriverTask();

#define HOUSEKEEPING_LENGTH 32 /*length of packed housekeeping*/
#define pushHousekeepingHeap(src) sendDataDriverCommand(PUSH_HEAP, src, 0, HOUSEKEEPING_LENGTH)
#define popHousekeepingHeap(dest, flag) sendDataDriverCommand(PUSH_HEAP, dest, 0, HOUSEKEEPING_LENGTH, flag)

#define writeConfig(src, offset, length) sendDataDriverCommand(WRITE_CONFIG, src, offset, length)
#define readConfig(dest, offset, length, flag) sendDataDriverCommand(READ_CONFIG, src, offset, length, flag)

    /*Data sizes and offsets*/

#define HEAP_BEGIN 0x400
#define CONFIG_BEGIN 0x400

#define FIRST_BOOT_FLAG_LENGTH 1
#define ANTENNA_DEPLOY_FLAG_LENGTH 1

#define FIRST_BOOT_FLAG_OFFSET (HEAP_BEGIN)
#define ANTENNA_DEPLOY_FLAG_OFFSET (FIRST_BOOT_FLAG_LENGTH + FIRST_BOOT_FLAG_OFFSET)
    




#ifdef	__cplusplus
}
#endif

#endif	/* DATA_H */

