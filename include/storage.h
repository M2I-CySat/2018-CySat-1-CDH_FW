/* 
 * File:   storage.h
 * Author: Jake Drahos
 *
 * Created on September 20, 2014, 1:22 PM
 *
 * Contains abstract data storage routines. This includes the data heap
 * as well as config storage. Once the data storage task is started,
 * it is illegal to directly call the mem read/write functions from any other
 * task.
 */

#ifndef STORAGE_H
#define	STORAGE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <power.h>
#include <FreeRTOS.h>

#define USE_IMPLEMENTATION_HEAP
// #define USE_IMPLEMENTATION_RINGBUFFER
    
    typedef enum {
        PUSH_HEAP,          //push OPERAND to heap, LENGTH is unit size
        POP_HEAP,           //pop from heap to OPERAND, LENGTH is unit size
        WRITE_CONFIG,       //copy LENGTH from OPERAND to OFFSET
        READ_CONFIG,        //copy LENGTH from OFFSET to OPERAND
        CLEAR_HEAP,         //Clear the heap
    } storageDriverCommandType;

    typedef struct {
        storageDriverCommandType command;
        unsigned char * operand;
        unsigned int offset;
        unsigned int length;
        unsigned char * completed; /*Flag set upon completion. Used to signal a completed read*/
    } storageDriverCommand;

#define HOUSEKEEPING_LENGTH 32 /*length of packed housekeeping*/
#define FOOTPRINT_DATA_SIZE 32
    
    void packHousekeeping(powerData *, unsigned char *);
    void unpackHousekeeping(unsigned char *, powerData *);

    void pushFootprintBlocking(unsigned char *);
    void popFootprintBlocking(unsigned char *);
    void writeConfigBlocking(unsigned char *, unsigned int, unsigned int);
    void readConfigBlocking(unsigned char *, unsigned int, unsigned int);

    /*queue up a command*/
    int sendStorageDriverCommand(storageDriverCommandType, unsigned char *, unsigned int, unsigned int, unsigned char *);

    void startStorageDriverTask();

#define pushFootprint(src, flag) sendStorageDriverCommand(PUSH_HEAP, src, 0, 0, flag)
#define popFootprint(dest, flag) sendStorageDriverCommand(POP_HEAP, dest, 0, 0, flag)

#define writeConfig(src, offset, length, flag) sendStorageDriverCommand(WRITE_CONFIG, src, offset, length, flag)
#define readConfig(dest, offset, length, flag) sendStorageDriverCommand(READ_CONFIG, dest, offset, length, flag)

#define clearHeap() sendStorageDriverCommand(CLEAR_HEAP, 0, 0, 0, 0)

    /*Data sizes and offsets*/

#define HEAP_BEGIN 0x400
#define CONFIG_BEGIN 0x00

#define FIRST_BOOT_FLAG_LENGTH 1
#define ANTENNA_DEPLOY_FLAG_LENGTH 1

#define FIRST_BOOT_FLAG_OFFSET (CONFIG_BEGIN)
#define ANTENNA_DEPLOY_FLAG_OFFSET (FIRST_BOOT_FLAG_LENGTH + FIRST_BOOT_FLAG_OFFSET)

    

    void storageTestTask();


#ifdef	__cplusplus
}
#endif

#endif	/* DATA_H */

