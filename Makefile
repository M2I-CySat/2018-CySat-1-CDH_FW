# Global project variables

# Select target
ifndef TARGET
	export TARGET=411
endif

# Set variables based on target
ifeq "$(TARGET)" "411"
	export MCU_MODEL=STM32F411xE
	export LDSCRIPT=stm32f411re.ld
  export STARTUP_OBJECT=startup_stm32f411xe.o
endif

ifeq "$(TARGET)" "417"
	export MCU_MODEL=STM32F40_41xxx
	export LDSCRIPT=stm32f417vg.ld
  export STARTUP_OBJECT=startup_stm32f417xx.o
endif

ifeq "$(TARGET)" "401"
	export MCU_MODEL=STM32F401xx
	export LDSCRIPT=stm32f401re.ld
endif

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
export CFLAGS= --specs=nosys.specs -mthumb -mcpu=cortex-m4 -mfloat-abi=softfp -mno-thumb-interwork -mfpu=vfp -msoft-float
export CFLAGS+= -mfpu=fpv4-sp-d16 -g -DUSE_STDPERIPH_DRIVER -D$(MCU_MODEL)
#Bitchy nag-nag mode settings
export CFLAGS+= -Wall -Wno-unused-function -Wno-pointer-sign -Werror
export ASFLAGS= -mthumb -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=vfp -mfpu=fpv4-sp-d16 -g

all:
	cd src && make

clean:
	cd src && make clean

image.elf: all

debug: src/image.elf
	arm-none-eabi-gdb src/image.elf -ex 'target extended localhost:4242' -ex 'load'
