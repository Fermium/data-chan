# Build

The entire project can be compiled with the `make all` command, even on windows, but it requires a lot of dependencies!


## Environment

On a generic system you will need at least:

* make
* gcc-avr
* avr-libc
* gcc
* g++
* libusb-1.0-0-dev
* libpthread-stubs0-dev (pre-installed on most systems)

	 
If you're using windows you will **also** need to install:

* msys 1.0
* [tdm-gcc](http://tdm-gcc.tdragon.net/)
* [grep for windows](http://www.wingrep.com/)


	
Either if you are using Linux, MacOS or Windows, if you plan to use the Atmega32u4 or any other
AVR microcontroller you will need a [USBASP](http://www.fischl.de/usbasp/) to program the microcontroller and [AVRDUDE](http://www.nongnu.org/avrdude/) to drive it.