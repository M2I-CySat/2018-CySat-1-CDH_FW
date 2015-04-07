# Add filename.o to include in compilation
OBJECTS=main.o system_stm32f4xx.o startup_stm32f411xe.o serial.o uart.o
OBJECTS+= command.o clock.o spi.o i2c.o


all: image.elf

%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

%.o: %.s
	$(AS) -c $(ASFLAGS) $< -o $@

  
image.elf: $(OBJECTS)
	cd STPeriph && make
	cd FreeRTOS && make
	$(CC) $(CFLAGS) $(CPPFLAGS) -T ../$(LDSCRIPT) -o image.elf *.o FreeRTOS/*.o STPeriph/*.o

clean:
	rm -f *.o
	cd FreeRTOS && make clean
	cd STPeriph && make clean
	rm -f image.elf
