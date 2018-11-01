# Useful commands
#	Kill all docker images
#		docker system prune -a 
#	Build our docker image	
#		docker build -t cysat .
#	Finally, run our image
#		docker run -it --privileged -v /dev/bus/usb:/dev/bus/usb --expose=3333 cysat:latest 


# Lets use arch, its much eaiser
FROM archlinux/base

# Set working directory
WORKDIR /app


# Install needed tools
RUN pacman -Sy --noconfirm arm-none-eabi-gcc arm-none-eabi-newlib arm-none-eabi-gdb git make tmux vim sudo wget confuse hidapi libftdi libftdi-compat libusb-compat m4 autoconf automake pkgconf gcc fakeroot 

# All of this nonsense is to get openocd to install because Arch does not trust their users and is stupid in some ways...
RUN chown -R nobody /app
USER nobody
RUN git clone https://git.code.sf.net/p/openocd/code openocd
RUN wget https://aur.archlinux.org/cgit/aur.git/snapshot/openocd.tar.gz
RUN tar xvzf openocd.tar.gz
WORKDIR /app/openocd
RUN chown -R nobody /app
USER root
RUN echo "nobody ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers
USER nobody
RUN makepkg -si --noconfirm

# Clone our cysat repo
RUN git clone https://github.com/M2I-CySat/CySat-1-CDH_FW.git /app/firmware
WORKDIR /app/firmware
RUN make

# I am groot!
USER root

# Udev rule for the Nucleo board
RUN echo 'ATTRS{idVendor}=="0483", ATTRS{idProduct}=="374b", MODE="664", GROUP="plugdev"' >> /etc/udev/rules.d/99-openocd.rules

# Humans do like names, dont they
ENV NAME cysat


# Move to directory with the script to run our tools
WORKDIR /app/firmware/Docker

# Finally, when the docker container run this script to start our tools
CMD "./cysat-dev.sh"
