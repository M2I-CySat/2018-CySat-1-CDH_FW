tmux new-session -d 'cysat-dev'
tmux split-window -v 'openocd -f /usr/share/openocd/scripts/board/st_nucleo_f4.cfg ; read'
tmux split-window -v 'arm-none-eabi-gdb /app/firmware/build/cysat.elf ; read'

