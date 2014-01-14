#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
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

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=src/FreeRTOS/croutine.c src/FreeRTOS/heap_1.c src/FreeRTOS/list.c src/FreeRTOS/port.c src/FreeRTOS/queue.c src/FreeRTOS/tasks.c src/FreeRTOS/timers.c src/FreeRTOS/portasm_PIC24.S src/comms.c src/helium.c src/iomapping.c src/myprintf.c src/payload.c src/power.c src/uart.c src/wire.c src/main.c src/serial.c src/nichrome.c src/testing.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/FreeRTOS/croutine.o ${OBJECTDIR}/src/FreeRTOS/heap_1.o ${OBJECTDIR}/src/FreeRTOS/list.o ${OBJECTDIR}/src/FreeRTOS/port.o ${OBJECTDIR}/src/FreeRTOS/queue.o ${OBJECTDIR}/src/FreeRTOS/tasks.o ${OBJECTDIR}/src/FreeRTOS/timers.o ${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o ${OBJECTDIR}/src/comms.o ${OBJECTDIR}/src/helium.o ${OBJECTDIR}/src/iomapping.o ${OBJECTDIR}/src/myprintf.o ${OBJECTDIR}/src/payload.o ${OBJECTDIR}/src/power.o ${OBJECTDIR}/src/uart.o ${OBJECTDIR}/src/wire.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/serial.o ${OBJECTDIR}/src/nichrome.o ${OBJECTDIR}/src/testing.o
POSSIBLE_DEPFILES=${OBJECTDIR}/src/FreeRTOS/croutine.o.d ${OBJECTDIR}/src/FreeRTOS/heap_1.o.d ${OBJECTDIR}/src/FreeRTOS/list.o.d ${OBJECTDIR}/src/FreeRTOS/port.o.d ${OBJECTDIR}/src/FreeRTOS/queue.o.d ${OBJECTDIR}/src/FreeRTOS/tasks.o.d ${OBJECTDIR}/src/FreeRTOS/timers.o.d ${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.d ${OBJECTDIR}/src/comms.o.d ${OBJECTDIR}/src/helium.o.d ${OBJECTDIR}/src/iomapping.o.d ${OBJECTDIR}/src/myprintf.o.d ${OBJECTDIR}/src/payload.o.d ${OBJECTDIR}/src/power.o.d ${OBJECTDIR}/src/uart.o.d ${OBJECTDIR}/src/wire.o.d ${OBJECTDIR}/src/main.o.d ${OBJECTDIR}/src/serial.o.d ${OBJECTDIR}/src/nichrome.o.d ${OBJECTDIR}/src/testing.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/FreeRTOS/croutine.o ${OBJECTDIR}/src/FreeRTOS/heap_1.o ${OBJECTDIR}/src/FreeRTOS/list.o ${OBJECTDIR}/src/FreeRTOS/port.o ${OBJECTDIR}/src/FreeRTOS/queue.o ${OBJECTDIR}/src/FreeRTOS/tasks.o ${OBJECTDIR}/src/FreeRTOS/timers.o ${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o ${OBJECTDIR}/src/comms.o ${OBJECTDIR}/src/helium.o ${OBJECTDIR}/src/iomapping.o ${OBJECTDIR}/src/myprintf.o ${OBJECTDIR}/src/payload.o ${OBJECTDIR}/src/power.o ${OBJECTDIR}/src/uart.o ${OBJECTDIR}/src/wire.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/serial.o ${OBJECTDIR}/src/nichrome.o ${OBJECTDIR}/src/testing.o

# Source Files
SOURCEFILES=src/FreeRTOS/croutine.c src/FreeRTOS/heap_1.c src/FreeRTOS/list.c src/FreeRTOS/port.c src/FreeRTOS/queue.c src/FreeRTOS/tasks.c src/FreeRTOS/timers.c src/FreeRTOS/portasm_PIC24.S src/comms.c src/helium.c src/iomapping.c src/myprintf.c src/payload.c src/power.c src/uart.c src/wire.c src/main.c src/serial.c src/nichrome.c src/testing.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/cysat.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ256GA110
MP_LINKER_FILE_OPTION=,--script=p24FJ256GA110.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/FreeRTOS/croutine.o: src/FreeRTOS/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/croutine.o.d 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/croutine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/croutine.c  -o ${OBJECTDIR}/src/FreeRTOS/croutine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/croutine.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/croutine.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/heap_1.o: src/FreeRTOS/heap_1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/heap_1.o.d 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/heap_1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/heap_1.c  -o ${OBJECTDIR}/src/FreeRTOS/heap_1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/heap_1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/heap_1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/list.o: src/FreeRTOS/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/list.o.d 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/list.c  -o ${OBJECTDIR}/src/FreeRTOS/list.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/list.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/list.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/port.o: src/FreeRTOS/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/port.o.d 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/port.c  -o ${OBJECTDIR}/src/FreeRTOS/port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/port.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/port.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/queue.o: src/FreeRTOS/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/queue.o.d 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/queue.c  -o ${OBJECTDIR}/src/FreeRTOS/queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/queue.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/queue.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/tasks.o: src/FreeRTOS/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/tasks.o.d 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/tasks.c  -o ${OBJECTDIR}/src/FreeRTOS/tasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/tasks.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/tasks.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/timers.o: src/FreeRTOS/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/timers.o.d 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/timers.c  -o ${OBJECTDIR}/src/FreeRTOS/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/timers.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/timers.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/comms.o: src/comms.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/comms.o.d 
	@${RM} ${OBJECTDIR}/src/comms.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/comms.c  -o ${OBJECTDIR}/src/comms.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/comms.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/comms.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/helium.o: src/helium.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/helium.o.d 
	@${RM} ${OBJECTDIR}/src/helium.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/helium.c  -o ${OBJECTDIR}/src/helium.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/helium.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/helium.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/iomapping.o: src/iomapping.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/iomapping.o.d 
	@${RM} ${OBJECTDIR}/src/iomapping.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/iomapping.c  -o ${OBJECTDIR}/src/iomapping.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/iomapping.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/iomapping.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/myprintf.o: src/myprintf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/myprintf.o.d 
	@${RM} ${OBJECTDIR}/src/myprintf.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/myprintf.c  -o ${OBJECTDIR}/src/myprintf.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/myprintf.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/myprintf.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/payload.o: src/payload.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/payload.o.d 
	@${RM} ${OBJECTDIR}/src/payload.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/payload.c  -o ${OBJECTDIR}/src/payload.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/payload.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/payload.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/power.o: src/power.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/power.o.d 
	@${RM} ${OBJECTDIR}/src/power.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/power.c  -o ${OBJECTDIR}/src/power.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/power.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/power.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/uart.o: src/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/uart.o.d 
	@${RM} ${OBJECTDIR}/src/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/uart.c  -o ${OBJECTDIR}/src/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/uart.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/wire.o: src/wire.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/wire.o.d 
	@${RM} ${OBJECTDIR}/src/wire.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/wire.c  -o ${OBJECTDIR}/src/wire.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/wire.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/wire.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/serial.o: src/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/serial.o.d 
	@${RM} ${OBJECTDIR}/src/serial.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/serial.c  -o ${OBJECTDIR}/src/serial.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/serial.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/serial.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/nichrome.o: src/nichrome.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/nichrome.o.d 
	@${RM} ${OBJECTDIR}/src/nichrome.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/nichrome.c  -o ${OBJECTDIR}/src/nichrome.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/nichrome.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/nichrome.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/testing.o: src/testing.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/testing.o.d 
	@${RM} ${OBJECTDIR}/src/testing.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/testing.c  -o ${OBJECTDIR}/src/testing.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/testing.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/testing.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/src/FreeRTOS/croutine.o: src/FreeRTOS/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/croutine.o.d 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/croutine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/croutine.c  -o ${OBJECTDIR}/src/FreeRTOS/croutine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/croutine.o.d"      -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/croutine.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/heap_1.o: src/FreeRTOS/heap_1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/heap_1.o.d 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/heap_1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/heap_1.c  -o ${OBJECTDIR}/src/FreeRTOS/heap_1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/heap_1.o.d"      -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/heap_1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/list.o: src/FreeRTOS/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/list.o.d 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/list.c  -o ${OBJECTDIR}/src/FreeRTOS/list.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/list.o.d"      -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/list.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/port.o: src/FreeRTOS/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/port.o.d 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/port.c  -o ${OBJECTDIR}/src/FreeRTOS/port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/port.o.d"      -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/port.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/queue.o: src/FreeRTOS/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/queue.o.d 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/queue.c  -o ${OBJECTDIR}/src/FreeRTOS/queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/queue.o.d"      -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/queue.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/tasks.o: src/FreeRTOS/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/tasks.o.d 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/tasks.c  -o ${OBJECTDIR}/src/FreeRTOS/tasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/tasks.o.d"      -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/tasks.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/timers.o: src/FreeRTOS/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/timers.o.d 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/timers.c  -o ${OBJECTDIR}/src/FreeRTOS/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/timers.o.d"      -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/timers.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/comms.o: src/comms.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/comms.o.d 
	@${RM} ${OBJECTDIR}/src/comms.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/comms.c  -o ${OBJECTDIR}/src/comms.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/comms.o.d"      -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/comms.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/helium.o: src/helium.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/helium.o.d 
	@${RM} ${OBJECTDIR}/src/helium.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/helium.c  -o ${OBJECTDIR}/src/helium.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/helium.o.d"      -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/helium.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/iomapping.o: src/iomapping.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/iomapping.o.d 
	@${RM} ${OBJECTDIR}/src/iomapping.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/iomapping.c  -o ${OBJECTDIR}/src/iomapping.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/iomapping.o.d"      -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/iomapping.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/myprintf.o: src/myprintf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/myprintf.o.d 
	@${RM} ${OBJECTDIR}/src/myprintf.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/myprintf.c  -o ${OBJECTDIR}/src/myprintf.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/myprintf.o.d"      -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/myprintf.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/payload.o: src/payload.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/payload.o.d 
	@${RM} ${OBJECTDIR}/src/payload.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/payload.c  -o ${OBJECTDIR}/src/payload.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/payload.o.d"      -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/payload.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/power.o: src/power.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/power.o.d 
	@${RM} ${OBJECTDIR}/src/power.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/power.c  -o ${OBJECTDIR}/src/power.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/power.o.d"      -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/power.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/uart.o: src/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/uart.o.d 
	@${RM} ${OBJECTDIR}/src/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/uart.c  -o ${OBJECTDIR}/src/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/uart.o.d"      -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/wire.o: src/wire.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/wire.o.d 
	@${RM} ${OBJECTDIR}/src/wire.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/wire.c  -o ${OBJECTDIR}/src/wire.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/wire.o.d"      -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/wire.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/main.o.d"      -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/serial.o: src/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/serial.o.d 
	@${RM} ${OBJECTDIR}/src/serial.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/serial.c  -o ${OBJECTDIR}/src/serial.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/serial.o.d"      -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/serial.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/nichrome.o: src/nichrome.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/nichrome.o.d 
	@${RM} ${OBJECTDIR}/src/nichrome.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/nichrome.c  -o ${OBJECTDIR}/src/nichrome.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/nichrome.o.d"      -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/nichrome.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/testing.o: src/testing.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/testing.o.d 
	@${RM} ${OBJECTDIR}/src/testing.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/testing.c  -o ${OBJECTDIR}/src/testing.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/testing.o.d"      -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/testing.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
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
	@${RM} ${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  src/FreeRTOS/portasm_PIC24.S  -o ${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.d"  -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -I"." -I"./include" -I"./include/FreeRTOS" -I"./src" -I"./src/FreeRTOS" -I"./src" -I"./src/FreeRTOS" -Wa,-MD,"${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.asm.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.d" "${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.asm.d"  -t $(SILENT)  -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o: src/FreeRTOS/portasm_PIC24.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.d 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  src/FreeRTOS/portasm_PIC24.S  -o ${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.d"  -omf=elf -I"." -I"./include" -I"./include/FreeRTOS" -I"./src" -I"./src/FreeRTOS" -I"./src" -I"./src/FreeRTOS" -Wa,-MD,"${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.asm.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.d" "${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.asm.d"  -t $(SILENT)  -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/cysat.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/cysat.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/cysat.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/cysat.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}/xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/cysat.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf 
	
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

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
