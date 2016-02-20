# Global project variables
# MCU Define
export MCU_MODEL=STM32F411xE

# Toolchain
export CC=arm-none-eabi-gcc
export AS=arm-none-eabi-as
export OBJDUMP=arg-none-eabi-objdump

# Drivers path
export DRIVERS=../Drivers

# Project root
export PROJECT_ROOT=./

# Include Paths
#export IFLAGS=-I$(DRIVERS)/BSP/STM32F4xx-Nucleo/
export IFLAGS+= -I$(DRIVERS)/CMSIS/Include/
export IFLAGS+= -I$(DRIVERS)/CMSIS/Device/ST/STM32F4xx/Include
export IFLAGS+= -I$(DRIVERS)/STM32F4xx_HAL_Driver/inc
export IFLAGS+= -I$(DRIVERS)/Device/ST/STM32F4xx/Include/
export IFLAGS+= -I$(PROJECT_ROOT)/Inc/

# Set flags
export CFLAGS= --specs=nosys.specs -mthumb -mcpu=cortex-m4 -mfloat-abi=hard
export CFLAGS+= -mfpu=fpv4-sp-d16 -g -D$(MCU_MODEL)

#Bitchy nag-nag mode settings
export CFLAGS+= -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -std=gnu11

build: link

all: hal-objects project-files link

hal-objects:
	$(CC) $(CFLAGS) $(IFLAGS) -c $(DRIVERS)/STM32F4xx_HAL_Driver/Src/*.c
	mv *.o objects/
	
bsp-objects:
	$(CC) $(CFLAGS) $(IFLAGS) -c $(DRIVERS)/BSP/STM32F4xx-Nucleo/*.c
	mv *.o objects
	
project-files:
	$(CC) $(CFLAGS) $(IFLAGS) -c $(PROJECT_ROOT)/src/*.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $(PROJECT_ROOT)/src/drivers/*.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $(PROJECT_ROOT)/startup_ARMCM4.S
	mv *.o objects
	
link: project-files
	$(CC) $(CFLAGS) -T stm32f411re.ld -o image.elf objects/*.o -L ./lib -lRTX_CM4
	
clean:
	rm -f image.elf objects/*.o
