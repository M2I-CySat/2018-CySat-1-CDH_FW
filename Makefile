# Global project variables
# MCU Define
MCU_MODEL=STM32F411xE
LDSCRIPT=STM32F411RETx_FLASH.ld

# Toolchain
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
GDB=arm-none-eabi-gdb

# Project root (using pwd breaks windows builds)
PROJECT_ROOT=.

# Drivers path
DRIVERS=$(PROJECT_ROOT)/Drivers

# Include Paths
IFLAGS=-I$(DRIVERS)/CMSIS/Include/
IFLAGS+=-I$(DRIVERS)/CMSIS/Device/ST/STM32F4xx/Include
IFLAGS+=-I$(DRIVERS)/STM32F4xx_HAL_Driver/Inc
IFLAGS+=-I$(PROJECT_ROOT)/Inc/

FREERTOS_ROOT=$(PROJECT_ROOT)/Middlewares/Third_Party/FreeRTOS/Source
FREERTOS_INCLUDES=CMSIS_RTOS include portable/GCC/ARM_CM4F/

IFLAGS+=$(foreach include,$(FREERTOS_INCLUDES),-I$(FREERTOS_ROOT)/$(include))

CPPFLAGS=$(IFLAGS) -D'__weak=__attribute((weak))'

# Set flags
CFLAGS=--specs=nosys.specs -mthumb -mcpu=cortex-m4 -mfloat-abi=hard
CFLAGS+=-mfpu=fpv4-sp-d16 -g -D$(MCU_MODEL)

# Bitchy nag-nag mode settings
CFLAGS+=-Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -std=gnu11

###### CONFIGURATION #######
# Startup
STARTUP=startup_stm32f411xe.s

# Source file definitions
APPLICATION_FILES=dma.c gpio.c main.c freertos.c i2c.c spi.c stm32f4xx_hal_msp.c
APPLICATION_FILES+=stm32f4xx_hal_timebase_TIM.c system_stm32f4xx.c stm32f4xx_it.c usart.c
APPLICATION_FILES+=callbacks.c uart2.c default_task.c heartbeat_task.c mem.c
APPLICATION_FILES+=$(STARTUP)

# HAL Requirements
HAL_MODULES=gpio uart rcc dma cortex spi i2c tim tim_ex

# Targets (build these things)
TARGETS=image.elf image.bin


###### END CONFIGURATION ######

HAL_FILES=stm32f4xx_hal.c $(foreach module,$(HAL_MODULES),stm32f4xx_hal_$(module).c)

FREERTOS_FILES=cmsis_os.c list.c queue.c tasks.c croutine.c event_groups.c timers.c port.c heap_4.c

APPLICATION_OBJECTS=$(subst .s,.o,$(subst .c,.o,$(APPLICATION_FILES)))
HAL_OBJECTS=$(subst .s,.o,$(subst .c,.o,$(HAL_FILES)))
FREERTOS_OBJECTS=$(subst .s,.o,$(subst .c,.o,$(FREERTOS_FILES)))

APPLICATION_OBJECTS_EXPANDED=$(foreach object,$(APPLICATION_OBJECTS),objects/$(object))
HAL_OBJECTS_EXPANDED=$(foreach object,$(HAL_OBJECTS),objects/hal/$(object))
FREERTOS_OBJECTS_EXPANDED=$(foreach object,$(FREERTOS_OBJECTS),objects/rtos/$(object))

ALL_OBJECTS=$(APPLICATION_OBJECTS_EXPANDED) $(HAL_OBJECTS_EXPANDED) $(FREERTOS_OBJECTS_EXPANDED)

# Buildoutputs
BUILDOUTPUTS=$(foreach output,$(TARGETS),output/$(output))

.PRECIOUS: $(ALL_OBJECTS)

all: $(BUILDOUTPUTS)

output/%.elf: $(ALL_OBJECTS)
	$(CC) $(CFLAGS) -T $(LDSCRIPT) $(ALL_OBJECTS) -o $@

output/%.bin: output/%.elf
	$(OBJCOPY) -O binary $< $@

objects/%.o: Src/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

objects/%.o: startup/%.s
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

objects/hal/%.o: $(DRIVERS)/STM32F4xx_HAL_Driver/Src/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

objects/rtos/%.o: $(FREERTOS_ROOT)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

objects/rtos/%.o: $(FREERTOS_ROOT)/CMSIS_RTOS/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

objects/rtos/%.o: $(FREERTOS_ROOT)/portable/GCC/ARM_CM4F/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

objects/rtos/%.o: $(FREERTOS_ROOT)/portable/MemMang/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(BUILDOUTPUTS) $(ALL_OBJECTS)

start-openocd:
	openocd -f openocd.cfg

debug: output/image.elf
	$(GDB) -ex "target extended localhost:3333" -ex "load" output/image.elf
