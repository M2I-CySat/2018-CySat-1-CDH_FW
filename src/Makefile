CC=arm-none-eabi-gcc
AS=arm-none-eabi-as
OBJDUMP=arg-none-eabi-objdump

OBJECTS=main.o system_stm32f4xx.o startup_stm32f411xe.o serial.o uart.o

CMSIS_ROOT=../CMSIS

IFLAGS=-I../include/
IFLAGS+=-I../include/FreeRTOS
IFLAGS+=-I../include/STPeriph
IFLAGS+= -I$(CMSIS_ROOT)/Device/ST/STM32F4xx/Include/
IFLAGS+= -I$(CMSIS_ROOT)/Include

CPPFLAGS+= $(IFLAGS)

CFLAGS= --specs=nosys.specs -mthumb -mcpu=cortex-m4 -mfloat-abi=hard
CFLAGS+= -mfpu=fpv4-sp-d16 -DSTM32F411xE -g -DUSE_STDPERIPH_DRIVER

ASFLAGS= -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g

all: image.elf

%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

%.o: %.s
	$(AS) -c $(ASFLAGS) $< -o $@
	
freertos:
	cd FreeRTOS && make
	cd STPeriph && make

image.elf: freertos $(OBJECTS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -T ../stm32f411re.ld -o image.elf *.o FreeRTOS/*.o STPeriph/*.o

clean:
	rm -f *.o
	cd FreeRTOS && make clean
	cd STPeriph && make clean
	rm -f image.elf