# Build

The entire project can be compiled with the "make all" command, even on windows, but it requires a lot of software!


## Environment

On a generic system you will need at least:
<ul>
make
	<li>gcc-avr</li>
	<li>avr-libc</li>
	<li>gcc</li>
	<li>g++</li>
	<li>libusb-1.0-0-dev</li>
	<li>libpthread-stubs0-dev (pre-installed on most systems)</li>
</ul>
	 
If you're using windows you will __also__ need to install:
<ul>
	<li>msys 1.0</li>
	<li>[tdm-gcc](http://tdm-gcc.tdragon.net/)</li>
	<li>[grep for windows](http://www.wingrep.com/)</li>
</ul>
	
Either if you are using Linux, Mac OSX or Windows, if you plan to use the Atmega332U4 or any other
AVR microcontroller you have to install avrdude and buy an usbasp device, because the bootloader will be
unable to operate again once the first write!