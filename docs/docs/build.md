# Build

The entire project can be compiled with the `make all` command, even on windows, but it requires a lot of dependencies.

At some point of the project lifetime lufa has to be fetched using submodule:

```sh
git submodule update --init --recursive
make all -j2
```

The -j2 flag is used to speed-up compilation time on multicore processors.


## Environment

On a generic system you will need at least:

* make
* gcc-avr (somewhere called avr-gcc)
* avr-libc
* gcc
* g++
* libusb-1.0-0-dev
* libpthread-stubs0-dev (pre-installed on most systems)

__NOTE:__ gcc and avr-libc used versions are 4.9, but any other greater versions should do a better job (gcc 6 compatibility is almost completely tested).

On Ubuntu/Debian run:

```sh
sudo apt install make gcc g++ libusb-1.0-0-dev gcc-avr avr-libc binutils-avr
```

On MacOS if you use Homebrew you have to tap an external repo:

```sh
brew tap osx-cross/avr
brew install avr-libc avr-gcc libusb
```

__NOTE:__ On MacOS if you don't have installed XCode you will need to:

```sh
brew install gcc
```

If you're using windows you will need to install:

* msys 1.0
* [tdm-gcc](http://tdm-gcc.tdragon.net/)
* [grep for windows](http://www.wingrep.com/)

If you are planning to use the Atmega32u4 or any other AVR microcontroller you will need an [USBASP](http://www.fischl.de/usbasp/) to program the MCU and [AVRDUDE](http://www.nongnu.org/avrdude/) to drive it.

You'll be able to use the firmware bootloader shipped with your device only once!
