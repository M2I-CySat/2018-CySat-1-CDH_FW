# Useful commands
#	docker system prune -a 
#	docker build .


# Lets use arch, its much eaiser
FROM archlinux/base

# Set working directory
WORKDIR /app


# Install needed tools
RUN pacman -Sy --noconfirm arm-none-eabi-gcc arm-none-eabi-newlib arm-none-eabi-gdb git make tmux vim sudo wget confuse hidapi libftdi libftdi-compat libusb-compat m4 autoconf automake pkgconf gcc fakeroot 

# All of this nonsense is to get openocd to install because Arch does not trust their users and is stupid in some ways...
RUN chown -R nobody /app
RUN sudo -u nobody git clone https://aur.archlinux.org/openocd.git
RUN wget https://aur.archlinux.org/cgit/aur.git/snapshot/openocd.tar.gz
RUN tar xvzf openocd.tar.gz
WORKDIR /app/openocd
RUN chown -R nobody /app
RUN echo "nobody ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers
USER nobody
RUN makepkg -si --noconfirm

# Clone our cysat repo
RUN git clone https://github.com/M2I-CySat/CySat-1-CDH_FW.git /app/firmware
WORKDIR /app/firmware
RUN make

USER root

# Humans do like names, dont they
ENV NAME cysat

# run /bin/bash when the container launches
# CMD ["bash"]

WORKDIR /app/firmware/Docker
# Lets see if I can get this pos to run our commands as is
#CMD "tmux new-session 'openocd -f /usr/share/openocd/scripts/board/st_nucleo_f4.cfg ; read' \; split-window 'arm-none-eabi-gdb /app/firmware/build/cysat.elf ; read'"
CMD "./cysat-dev.sh"
