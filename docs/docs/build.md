# Build

The entire project can be compiled with the "make all" command, even on windows, but it requires a lot of software!


## Environment

On a generic system you will need at least:

     * make
     * gcc-avr
	 * avr-libc
	 * gcc
     * g++
	 * libusb-1.0-0-dev
	 * libpthread-stubs0-dev (pre-installed on most systems)
	 
If you're using windows you will __also__ need to install:
     
	 * msys 1.0
	 * [tdm-gcc](http://tdm-gcc.tdragon.net/)
	 * [grep for windows](http://www.wingrep.com/)
	 
Either if you are using Linux, Mac OSX or Windows, if you plan to use the Atmega332U4 or any other
AVR microcontroller you have to install avrdude and buy an usbasp device, because the bootloader will be
unable to operate again once the first write!