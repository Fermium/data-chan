# Build

The entire project can be compiled with the `make all` command, even on windows, but it requires a lot of dependencies!


## Environment

On a generic system you will need at least:

* make
* gcc-avr (somewhere called avr-gcc)
* avr-libc
* gcc
* g++
* libusb-1.0-0-dev
* libpthread-stubs0-dev (pre-installed on most systems)

__NOTE:__ gcc and avr-libc used versions are 4.9, but any other greater versions
should do a better job (gcc 6 compatibility is mostly tested).

On Ubuntu/Debian run:

```sh
sudo apt install pkg-config libssl-dev wget libusb-1.0-0-dev zlib1g-dev unzip openssh-client unzip tar maven gcc g++ gcc-avr avr-libc binutils-avr make
```

On MacOS if you use Homebrew you have to tap an external repo:
```sh
brew tap osx-cross/avr
brew install avr-libc avr-gcc gcc libusb
```


If you're using windows you will **also** need to install:

* msys 1.0
* [tdm-gcc](http://tdm-gcc.tdragon.net/)
* [grep for windows](http://www.wingrep.com/)

Either if you are using Linux, MacOS or Windows, if you plan to use the Atmega32u4 or any other
AVR microcontroller you will need a [USBASP](http://www.fischl.de/usbasp/) to program the microcontroller and [AVRDUDE](http://www.nongnu.org/avrdude/) to drive it.

Because you'll be able to use the pre-burned bootloader only once!
