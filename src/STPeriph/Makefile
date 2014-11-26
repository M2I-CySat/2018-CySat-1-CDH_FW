CC=arm-none-eabi-gcc
AS=arm-none-eabi-as
OBJDUMP=arg-none-eabi-objdump

OBJECTS=misc.o stm32f4xx_gpio.o stm32f4xx_rcc.o

CMSIS_ROOT=/opt/cross/stm32f4/Drivers/CMSIS

IFLAGS=-I../../include/
IFLAGS+=-I../../include/FreeRTOS
IFLAGS+=-I../../include/STPeriph
IFLAGS+= -I$(CMSIS_ROOT)/Device/ST/STM32F4xx/Include/
IFLAGS+= -I$(CMSIS_ROOT)/Include

CPPFLAGS+= $(IFLAGS)

CFLAGS= --specs=nosys.specs -mthumb -mcpu=cortex-m4 -mfloat-abi=hard
CFLAGS+= -mfpu=fpv4-sp-d16 -DSTM32F411xE -g

ASFLAGS= -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g

%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

%.o: %.s
	$(AS) -c $(ASFLAGS) $< -o $@

all: $(OBJECTS)

clean:
	rm -f *.o