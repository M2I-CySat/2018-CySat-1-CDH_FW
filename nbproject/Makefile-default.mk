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
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/FreeRTOS/croutine.o ${OBJECTDIR}/src/FreeRTOS/heap_1.o ${OBJECTDIR}/src/FreeRTOS/list.o ${OBJECTDIR}/src/FreeRTOS/port.o ${OBJECTDIR}/src/FreeRTOS/queue.o ${OBJECTDIR}/src/FreeRTOS/tasks.o ${OBJECTDIR}/src/FreeRTOS/timers.o ${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/serial.o ${OBJECTDIR}/uart.o ${OBJECTDIR}/testing.o ${OBJECTDIR}/iomapping.o ${OBJECTDIR}/power.o ${OBJECTDIR}/wire.o ${OBJECTDIR}/payload.o ${OBJECTDIR}/rtc.o ${OBJECTDIR}/helium.o ${OBJECTDIR}/comms.o ${OBJECTDIR}/myprintf.o
POSSIBLE_DEPFILES=${OBJECTDIR}/src/FreeRTOS/croutine.o.d ${OBJECTDIR}/src/FreeRTOS/heap_1.o.d ${OBJECTDIR}/src/FreeRTOS/list.o.d ${OBJECTDIR}/src/FreeRTOS/port.o.d ${OBJECTDIR}/src/FreeRTOS/queue.o.d ${OBJECTDIR}/src/FreeRTOS/tasks.o.d ${OBJECTDIR}/src/FreeRTOS/timers.o.d ${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.d ${OBJECTDIR}/src/main.o.d ${OBJECTDIR}/src/serial.o.d ${OBJECTDIR}/uart.o.d ${OBJECTDIR}/testing.o.d ${OBJECTDIR}/iomapping.o.d ${OBJECTDIR}/power.o.d ${OBJECTDIR}/wire.o.d ${OBJECTDIR}/payload.o.d ${OBJECTDIR}/rtc.o.d ${OBJECTDIR}/helium.o.d ${OBJECTDIR}/comms.o.d ${OBJECTDIR}/myprintf.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/FreeRTOS/croutine.o ${OBJECTDIR}/src/FreeRTOS/heap_1.o ${OBJECTDIR}/src/FreeRTOS/list.o ${OBJECTDIR}/src/FreeRTOS/port.o ${OBJECTDIR}/src/FreeRTOS/queue.o ${OBJECTDIR}/src/FreeRTOS/tasks.o ${OBJECTDIR}/src/FreeRTOS/timers.o ${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/serial.o ${OBJECTDIR}/uart.o ${OBJECTDIR}/testing.o ${OBJECTDIR}/iomapping.o ${OBJECTDIR}/power.o ${OBJECTDIR}/wire.o ${OBJECTDIR}/payload.o ${OBJECTDIR}/rtc.o ${OBJECTDIR}/helium.o ${OBJECTDIR}/comms.o ${OBJECTDIR}/myprintf.o


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

MP_PROCESSOR_OPTION=24FJ256GA110
MP_LINKER_FILE_OPTION=,--script=p24FJ256GA110.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/FreeRTOS/croutine.o: src/FreeRTOS/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/croutine.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/croutine.c  -o ${OBJECTDIR}/src/FreeRTOS/croutine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/croutine.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/croutine.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/heap_1.o: src/FreeRTOS/heap_1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/heap_1.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/heap_1.c  -o ${OBJECTDIR}/src/FreeRTOS/heap_1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/heap_1.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/heap_1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/list.o: src/FreeRTOS/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/list.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/list.c  -o ${OBJECTDIR}/src/FreeRTOS/list.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/list.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/list.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/port.o: src/FreeRTOS/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/port.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/port.c  -o ${OBJECTDIR}/src/FreeRTOS/port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/port.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/port.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/queue.o: src/FreeRTOS/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/queue.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/queue.c  -o ${OBJECTDIR}/src/FreeRTOS/queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/queue.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/queue.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/tasks.o: src/FreeRTOS/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/tasks.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/tasks.c  -o ${OBJECTDIR}/src/FreeRTOS/tasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/tasks.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/tasks.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/timers.o: src/FreeRTOS/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/timers.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/timers.c  -o ${OBJECTDIR}/src/FreeRTOS/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/timers.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/timers.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/main.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/serial.o: src/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/serial.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/serial.c  -o ${OBJECTDIR}/src/serial.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/serial.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/serial.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/uart.o: uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/uart.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uart.c  -o ${OBJECTDIR}/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/uart.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/testing.o: testing.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/testing.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  testing.c  -o ${OBJECTDIR}/testing.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/testing.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/testing.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/iomapping.o: iomapping.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/iomapping.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  iomapping.c  -o ${OBJECTDIR}/iomapping.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/iomapping.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/iomapping.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/power.o: power.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/power.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  power.c  -o ${OBJECTDIR}/power.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/power.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/power.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/wire.o: wire.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/wire.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  wire.c  -o ${OBJECTDIR}/wire.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/wire.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/wire.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/payload.o: payload.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/payload.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  payload.c  -o ${OBJECTDIR}/payload.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/payload.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/payload.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/rtc.o: rtc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/rtc.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  rtc.c  -o ${OBJECTDIR}/rtc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/rtc.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/rtc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/helium.o: helium.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/helium.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  helium.c  -o ${OBJECTDIR}/helium.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/helium.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/helium.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/comms.o: comms.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/comms.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  comms.c  -o ${OBJECTDIR}/comms.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/comms.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/comms.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/myprintf.o: myprintf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/myprintf.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  myprintf.c  -o ${OBJECTDIR}/myprintf.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/myprintf.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/myprintf.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/src/FreeRTOS/croutine.o: src/FreeRTOS/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/croutine.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/croutine.c  -o ${OBJECTDIR}/src/FreeRTOS/croutine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/croutine.o.d"        -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/croutine.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/heap_1.o: src/FreeRTOS/heap_1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/heap_1.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/heap_1.c  -o ${OBJECTDIR}/src/FreeRTOS/heap_1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/heap_1.o.d"        -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/heap_1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/list.o: src/FreeRTOS/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/list.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/list.c  -o ${OBJECTDIR}/src/FreeRTOS/list.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/list.o.d"        -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/list.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/port.o: src/FreeRTOS/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/port.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/port.c  -o ${OBJECTDIR}/src/FreeRTOS/port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/port.o.d"        -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/port.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/queue.o: src/FreeRTOS/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/queue.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/queue.c  -o ${OBJECTDIR}/src/FreeRTOS/queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/queue.o.d"        -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/queue.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/tasks.o: src/FreeRTOS/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/tasks.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/tasks.c  -o ${OBJECTDIR}/src/FreeRTOS/tasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/tasks.o.d"        -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/tasks.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/FreeRTOS/timers.o: src/FreeRTOS/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/timers.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/FreeRTOS/timers.c  -o ${OBJECTDIR}/src/FreeRTOS/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/timers.o.d"        -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/timers.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/main.o.d"        -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/serial.o: src/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/serial.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/serial.c  -o ${OBJECTDIR}/src/serial.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/serial.o.d"        -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/src/serial.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/uart.o: uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/uart.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uart.c  -o ${OBJECTDIR}/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/uart.o.d"        -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/testing.o: testing.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/testing.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  testing.c  -o ${OBJECTDIR}/testing.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/testing.o.d"        -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/testing.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/iomapping.o: iomapping.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/iomapping.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  iomapping.c  -o ${OBJECTDIR}/iomapping.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/iomapping.o.d"        -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/iomapping.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/power.o: power.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/power.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  power.c  -o ${OBJECTDIR}/power.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/power.o.d"        -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/power.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/wire.o: wire.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/wire.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  wire.c  -o ${OBJECTDIR}/wire.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/wire.o.d"        -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/wire.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/payload.o: payload.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/payload.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  payload.c  -o ${OBJECTDIR}/payload.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/payload.o.d"        -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/payload.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/rtc.o: rtc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/rtc.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  rtc.c  -o ${OBJECTDIR}/rtc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/rtc.o.d"        -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/rtc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/helium.o: helium.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/helium.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  helium.c  -o ${OBJECTDIR}/helium.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/helium.o.d"        -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/helium.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/comms.o: comms.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/comms.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  comms.c  -o ${OBJECTDIR}/comms.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/comms.o.d"        -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/comms.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/myprintf.o: myprintf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/myprintf.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  myprintf.c  -o ${OBJECTDIR}/myprintf.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/myprintf.o.d"        -g -omf=elf -O0 -I"." -I"./include" -I"./include/FreeRTOS" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off -fno-omit-frame-pointer
	@${FIXDEPS} "${OBJECTDIR}/myprintf.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
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
	${MP_CC} $(MP_EXTRA_AS_PRE)  src/FreeRTOS/portasm_PIC24.S  -o ${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.d"  -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -I"." -I"./include" -I"./include/FreeRTOS" -I"./src" -I"./src/FreeRTOS" -I"./src" -I"./src/FreeRTOS" -Wa,-MD,"${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.asm.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,-g,--no-relax,-g$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.d" "${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.asm.d"  -t $(SILENT)  -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o: src/FreeRTOS/portasm_PIC24.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/FreeRTOS 
	@${RM} ${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.d 
	${MP_CC} $(MP_EXTRA_AS_PRE)  src/FreeRTOS/portasm_PIC24.S  -o ${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.d"  -omf=elf -I"." -I"./include" -I"./include/FreeRTOS" -I"./src" -I"./src/FreeRTOS" -I"./src" -I"./src/FreeRTOS" -Wa,-MD,"${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.asm.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax,-g$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.d" "${OBJECTDIR}/src/FreeRTOS/portasm_PIC24.o.asm.d"  -t $(SILENT)  -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/cysat.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/cysat.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,$(MP_LINKER_FILE_OPTION),--heap=0,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--defsym,__ICD2RAM=1,--library-path=".",--no-force-link,--smart-io$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/cysat.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/cysat.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--heap=0,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--defsym,__ICD2RAM=1,--library-path=".",--no-force-link,--smart-io$(MP_EXTRA_LD_POST) 
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
