tmux new-session 'openocd -f /usr/share/openocd/scripts/board/st_nucleo_f4.cfg ; read' \; \
	split-window 'arm-none-eabi-gdb /app/firmware/build/cysat.elf ; read'

