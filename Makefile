all:
	cd src && make

clean:
	cd src && make clean

image.elf: all

debug: src/image.elf
	arm-none-eabi-gdb src/image.elf -ex 'target extended localhost:4242' -ex 'load'
