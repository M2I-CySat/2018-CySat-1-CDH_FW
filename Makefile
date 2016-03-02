# Global project variables
# MCU Define
export MCU_MODEL=STM32F411xE

# Toolchain
export CC=arm-none-eabi-gcc
export AS=arm-none-eabi-as
export OBJDUMP=arg-none-eabi-objdump

# Project root (using pwd breaks windows builds)
export PROJECT_ROOT=.

# Drivers path
export DRIVERS=$(PROJECT_ROOT)/vendor/Drivers

# Include Paths
#export IFLAGS=-I$(DRIVERS)/BSP/STM32F4xx-Nucleo/
export IFLAGS+= -I$(DRIVERS)/CMSIS/Include/
export IFLAGS+= -I$(DRIVERS)/CMSIS/Device/ST/STM32F4xx/Include
export IFLAGS+= -I$(DRIVERS)/STM32F4xx_HAL_Driver/Inc
export IFLAGS+= -I$(DRIVERS)/Device/ST/STM32F4xx/Include/
export IFLAGS+= -I$(PROJECT_ROOT)/inc/

export CPPFLAGS= $(IFLAGS)

# Set flags
export CFLAGS= --specs=nosys.specs -mthumb -mcpu=cortex-m4 -mfloat-abi=hard
export CFLAGS+= -mfpu=fpv4-sp-d16 -g -D$(MCU_MODEL)

# Bitchy nag-nag mode settings
export CFLAGS+= -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -std=gnu11

# Source file definitions

# Files is src
export APPLICATION_FILES=main.c main.c printf.c error.c RTX_Conf_CM.c init.c

# Files in src/drivers
export DRIVER_FILES=uart.c stm32f4xx_it.c system_stm32f4xx.c

# HAL Requirements

# Startup
export STARTUP=startup_STM32F411.S
export STARTUP_OBJECT=objects/startup_STM32F411.o

# All ojbects
export ALL_OBJECTS=objects/*.o objects/hal/*.o objects/drivers/*.o

export APPLICATION_OBJECTS=$(subst .c,.o,$(APPLICATION_FILES))
export DRIVER_OBJECTS=$(subst .c,.o,$(DRIVER_FILES))

export APPLICATION_OBJECTS_EXPANDED= $(foreach object,$(APPLICATION_OBJECTS),objects/$(object))
export DRIVER_OBJECTS_EXPANDED= $(foreach object,$(DRIVER_OBJECTS),objects/drivers/$(object))

export HAL_FLAG= objects/hal_built

all: hal applications drivers startup
	$(CC) $(CFLAGS) -T stm32f411re.ld -o image.elf $(ALL_OBJECTS) -L $(PROJECT_ROOT)/vendor/lib -lRTX_CM4
	
hal: $(HAL_FLAG)

applications: $(APPLICATION_OBJECTS_EXPANDED)

drivers: $(DRIVER_OBJECTS_EXPANDED)

startup: $(STARTUP_OBJECT)

objects/%.o: src/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

objects/drivers/%.o: src/drivers/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(STARTUP_OBJECT): $(STARTUP)
	$(CC) $(CFLAGS) $(IFLAGS) -c $(STARTUP) -o $(STARTUP_OBJECT)

$(HAL_FLAG):
	$(CC) $(CFLAGS) $(IFLAGS) -c $(DRIVERS)/STM32F4xx_HAL_Driver/Src/*.c
	mv *.o objects/hal/
	touch objects/hal_built
	
clean:
	rm -f image.elf $(ALL_OBJECTS) objects/hal_built
