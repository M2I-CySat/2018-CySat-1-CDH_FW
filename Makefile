all:
	cd src && make

clean:
	cd src && make clean

image.elf: all

debug: src/image.elf
	gdb src/image.elf -ex 'target extended localhost:4242' -ex 'load'
