# Add filename.o to include in compilation
OBJECTS=list.o port.o queue.o tasks.o heap_1.o timers.o


%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

%.o: %.s
	$(AS) -c $(ASFLAGS) $< -o $@

all: $(OBJECTS)

clean:
	rm -f *.o