# Global project variables
# MCU Define
export MCU_MODEL=STM32F411xE

# Toolchain
export CC=arm-none-eabi-gcc
export AS=arm-none-eabi-as
export OBJDUMP=arg-none-eabi-objdump

#Project root (Complicated because Windows)
ifndef PROJECT_ROOT
	ifeq ($(shell printenv OS),Windows_NT)
		export PROJECT_ROOT=$(shell cygpath -ma .)
	else
		export PROJECT_ROOT=$(shell pwd)
	endif
endif

export PROJECT_ROOT

# Path to CMSIS Headers
export CMSIS_ROOT=$(PROJECT_ROOT)/CMSIS

# Include Paths
export IFLAGS=-I$(PROJECT_ROOT)/include/
export IFLAGS+= -I$(PROJECT_ROOT)/include/FreeRTOS/
export IFLAGS+= -I$(PROJECT_ROOT)/include/STPeriph/
export IFLAGS+= -I$(CMSIS_ROOT)/Device/ST/STM32F4xx/Include/
export IFLAGS+= -I$(CMSIS_ROOT)/Include/

# Set flags
export CPPFLAGS+= $(IFLAGS)
export CFLAGS= --specs=nosys.specs -mthumb -mcpu=cortex-m4 -mfloat-abi=hard
export CFLAGS+= -mfpu=fpv4-sp-d16 -g -DUSE_STDPERIPH_DRIVER -D$(MCU_MODEL)
#Bitchy nag-nag mode settings
export CFLAGS+= -Wall -Wno-unused-function -Wno-pointer-sign -Werror
export ASFLAGS= -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g

all:
	cd src && make

clean:
	cd src && make clean

image.elf: all

debug: src/image.elf
	arm-none-eabi-gdb src/image.elf -ex 'target extended localhost:4242' -ex 'load'
