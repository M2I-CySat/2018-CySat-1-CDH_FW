#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/cysat.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/cysat.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/FreeRTOS/croutine.o ${OBJECTDIR}/src/FreeRTOS/heap_1.o ${OBJECTDIR}/src/FreeRTOS/list.o ${OBJECTDIR}/src/FreeRTOS/port.o ${OBJECTDIR}/src/FreeRTOS/queue.o ${OBJECTDIR}/src/FreeRTOS/tasks.o ${OBJECTDIR}/src/FreeRTOS/timers.o ${OBJECTDIR}/src/lcd.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/ParTest.o ${OBJECTDIR}/src/serial.o ${OBJECTDIR}/src/timertest.o ${OBJECTDIR}/src/BlockQ.o ${OBJECTDIR}/src/blocktim.o ${OBJECTDIR}/src/comtest.o ${OBJECTDIR}/src/crflash.o ${OBJECTDIR}/src/integer.o ${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o
POSSIBLE_DEPFILES=${OBJECTDIR}/src/FreeRTOS/croutine.o.d ${OBJECTDIR}/src/FreeRTOS/heap_1.o.d ${OBJECTDIR}/src/FreeRTOS/list.o.d ${OBJECTDIR}/src/FreeRTOS/port.o.d ${OBJECTDIR}/src/FreeRTOS/queue.o.d ${OBJECTDIR}/src/FreeRTOS/tasks.o.d ${OBJECTDIR}/src/FreeRTOS/timers.o.d ${OBJECTDIR}/src/lcd.o.d ${OBJECTDIR}/src/main.o.d ${OBJECTDIR}/src/ParTest.o.d ${OBJECTDIR}/src/serial.o.d ${OBJECTDIR}/src/timertest.o.d ${OBJECTDIR}/src/BlockQ.o.d ${OBJECTDIR}/src/blocktim.o.d ${OBJECTDIR}/src/comtest.o.d ${OBJECTDIR}/src/crflash.o.d ${OBJECTDIR}/src/integer.o.d ${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/FreeRTOS/croutine.o ${OBJECTDIR}/src/FreeRTOS/heap_1.o ${OBJECTDIR}/src/FreeRTOS/list.o ${OBJECTDIR}/src/FreeRTOS/port.o ${OBJECTDIR}/src/FreeRTOS/queue.o ${OBJECTDIR}/src/FreeRTOS/tasks.o ${OBJECTDIR}/src/FreeRTOS/timers.o ${OBJECTDIR}/src/lcd.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/ParTest.o ${OBJECTDIR}/src/serial.o ${OBJECTDIR}/src/timertest.o ${OBJECTDIR}/src/BlockQ.o ${OBJECTDIR}/src/blocktim.o ${OBJECTDIR}/src/comtest.o ${OBJECTDIR}/src/crflash.o ${OBJECTDIR}/src/integer.o ${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/cysat.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ128GA010
MP_LINKER_FILE_OPTION=,--script="p24FJ128GA010.gld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/FreeRTOS/croutine.o: src/FreeRTOS/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/croutine.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/croutine.c  -o ${OBJECTDIR}/src/FreeRTOS/croutine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/croutine.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/croutine.o.d" $(SILENT) 
	
${OBJECTDIR}/src/FreeRTOS/heap_1.o: src/FreeRTOS/heap_1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/heap_1.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/heap_1.c  -o ${OBJECTDIR}/src/FreeRTOS/heap_1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/heap_1.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/heap_1.o.d" $(SILENT) 
	
${OBJECTDIR}/src/FreeRTOS/list.o: src/FreeRTOS/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/list.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/list.c  -o ${OBJECTDIR}/src/FreeRTOS/list.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/list.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/list.o.d" $(SILENT) 
	
${OBJECTDIR}/src/FreeRTOS/port.o: src/FreeRTOS/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/port.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/port.c  -o ${OBJECTDIR}/src/FreeRTOS/port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/port.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/port.o.d" $(SILENT) 
	
${OBJECTDIR}/src/FreeRTOS/queue.o: src/FreeRTOS/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/queue.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/queue.c  -o ${OBJECTDIR}/src/FreeRTOS/queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/queue.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/queue.o.d" $(SILENT) 
	
${OBJECTDIR}/src/FreeRTOS/tasks.o: src/FreeRTOS/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/tasks.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/tasks.c  -o ${OBJECTDIR}/src/FreeRTOS/tasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/tasks.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/tasks.o.d" $(SILENT) 
	
${OBJECTDIR}/src/FreeRTOS/timers.o: src/FreeRTOS/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/timers.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/timers.c  -o ${OBJECTDIR}/src/FreeRTOS/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/timers.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/timers.o.d" $(SILENT) 
	
${OBJECTDIR}/src/lcd.o: src/lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/lcd.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/lcd.c  -o ${OBJECTDIR}/src/lcd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/lcd.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/lcd.o.d" $(SILENT) 
	
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/main.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT) 
	
${OBJECTDIR}/src/ParTest.o: src/ParTest.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/ParTest.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/ParTest.c  -o ${OBJECTDIR}/src/ParTest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/ParTest.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/ParTest.o.d" $(SILENT) 
	
${OBJECTDIR}/src/serial.o: src/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/serial.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/serial.c  -o ${OBJECTDIR}/src/serial.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/serial.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/serial.o.d" $(SILENT) 
	
${OBJECTDIR}/src/timertest.o: src/timertest.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/timertest.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/timertest.c  -o ${OBJECTDIR}/src/timertest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/timertest.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/timertest.o.d" $(SILENT) 
	
${OBJECTDIR}/src/BlockQ.o: src/BlockQ.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/BlockQ.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/BlockQ.c  -o ${OBJECTDIR}/src/BlockQ.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/BlockQ.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/BlockQ.o.d" $(SILENT) 
	
${OBJECTDIR}/src/blocktim.o: src/blocktim.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/blocktim.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/blocktim.c  -o ${OBJECTDIR}/src/blocktim.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/blocktim.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/blocktim.o.d" $(SILENT) 
	
${OBJECTDIR}/src/comtest.o: src/comtest.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/comtest.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/comtest.c  -o ${OBJECTDIR}/src/comtest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/comtest.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/comtest.o.d" $(SILENT) 
	
${OBJECTDIR}/src/crflash.o: src/crflash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/crflash.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/crflash.c  -o ${OBJECTDIR}/src/crflash.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/crflash.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/crflash.o.d" $(SILENT) 
	
${OBJECTDIR}/src/integer.o: src/integer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/integer.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/integer.c  -o ${OBJECTDIR}/src/integer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/integer.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/integer.o.d" $(SILENT) 
	
else
${OBJECTDIR}/src/FreeRTOS/croutine.o: src/FreeRTOS/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/croutine.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/croutine.c  -o ${OBJECTDIR}/src/FreeRTOS/croutine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/croutine.o.d"        -g -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/croutine.o.d" $(SILENT) 
	
${OBJECTDIR}/src/FreeRTOS/heap_1.o: src/FreeRTOS/heap_1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/heap_1.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/heap_1.c  -o ${OBJECTDIR}/src/FreeRTOS/heap_1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/heap_1.o.d"        -g -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/heap_1.o.d" $(SILENT) 
	
${OBJECTDIR}/src/FreeRTOS/list.o: src/FreeRTOS/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/list.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/list.c  -o ${OBJECTDIR}/src/FreeRTOS/list.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/list.o.d"        -g -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/list.o.d" $(SILENT) 
	
${OBJECTDIR}/src/FreeRTOS/port.o: src/FreeRTOS/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/port.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/port.c  -o ${OBJECTDIR}/src/FreeRTOS/port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/port.o.d"        -g -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/port.o.d" $(SILENT) 
	
${OBJECTDIR}/src/FreeRTOS/queue.o: src/FreeRTOS/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/queue.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/queue.c  -o ${OBJECTDIR}/src/FreeRTOS/queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/queue.o.d"        -g -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/queue.o.d" $(SILENT) 
	
${OBJECTDIR}/src/FreeRTOS/tasks.o: src/FreeRTOS/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/tasks.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/tasks.c  -o ${OBJECTDIR}/src/FreeRTOS/tasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/tasks.o.d"        -g -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/tasks.o.d" $(SILENT) 
	
${OBJECTDIR}/src/FreeRTOS/timers.o: src/FreeRTOS/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/timers.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/timers.c  -o ${OBJECTDIR}/src/FreeRTOS/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/timers.o.d"        -g -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/timers.o.d" $(SILENT) 
	
${OBJECTDIR}/src/lcd.o: src/lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/lcd.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/lcd.c  -o ${OBJECTDIR}/src/lcd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/lcd.o.d"        -g -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/lcd.o.d" $(SILENT) 
	
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/main.o.d"        -g -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT) 
	
${OBJECTDIR}/src/ParTest.o: src/ParTest.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/ParTest.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/ParTest.c  -o ${OBJECTDIR}/src/ParTest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/ParTest.o.d"        -g -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/ParTest.o.d" $(SILENT) 
	
${OBJECTDIR}/src/serial.o: src/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/serial.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/serial.c  -o ${OBJECTDIR}/src/serial.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/serial.o.d"        -g -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/serial.o.d" $(SILENT) 
	
${OBJECTDIR}/src/timertest.o: src/timertest.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/timertest.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/timertest.c  -o ${OBJECTDIR}/src/timertest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/timertest.o.d"        -g -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/timertest.o.d" $(SILENT) 
	
${OBJECTDIR}/src/BlockQ.o: src/BlockQ.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/BlockQ.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/BlockQ.c  -o ${OBJECTDIR}/src/BlockQ.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/BlockQ.o.d"        -g -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/BlockQ.o.d" $(SILENT) 
	
${OBJECTDIR}/src/blocktim.o: src/blocktim.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/blocktim.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/blocktim.c  -o ${OBJECTDIR}/src/blocktim.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/blocktim.o.d"        -g -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/blocktim.o.d" $(SILENT) 
	
${OBJECTDIR}/src/comtest.o: src/comtest.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/comtest.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/comtest.c  -o ${OBJECTDIR}/src/comtest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/comtest.o.d"        -g -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/comtest.o.d" $(SILENT) 
	
${OBJECTDIR}/src/crflash.o: src/crflash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/crflash.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/crflash.c  -o ${OBJECTDIR}/src/crflash.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/crflash.o.d"        -g -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/crflash.o.d" $(SILENT) 
	
${OBJECTDIR}/src/integer.o: src/integer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/integer.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/integer.c  -o ${OBJECTDIR}/src/integer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/integer.o.d"        -g -omf=elf -O0 -I"." -I".\include" -I".\include\FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/integer.o.d" $(SILENT) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o: src/FreeRTOS/portasm_PIC24.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.d 
	${MP_CC} $(MP_EXTRA_AS_PRE)  src/FreeRTOS/portasm_PIC24.S  -o ${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.d"  -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -I"." -I".\include" -I".\include\FreeRTOS" -I".\src" -I".\src\FreeRTOS" -I".\src" -I".\src\FreeRTOS" -Wa,-MD,"${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.asm.d",--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,-g,--no-relax,-g$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.d" "${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.asm.d"  -t $(SILENT) 
	
else
${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o: src/FreeRTOS/portasm_PIC24.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.d 
	${MP_CC} $(MP_EXTRA_AS_PRE)  src/FreeRTOS/portasm_PIC24.S  -o ${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.d"  -omf=elf -I"." -I".\include" -I".\include\FreeRTOS" -I".\src" -I".\src\FreeRTOS" -I".\src" -I".\src\FreeRTOS" -Wa,-MD,"${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.asm.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax,-g$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.d" "${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.asm.d"  -t $(SILENT) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/cysat.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/cysat.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--defsym,__ICD2RAM=1,--no-force-link,--smart-io,--report-mem$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/cysat.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/cysat.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--defsym,__ICD2RAM=1,--no-force-link,--smart-io,--report-mem$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/cysat.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
