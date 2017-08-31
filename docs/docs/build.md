# Build

The entire project can be compiled with the `make debug` or `make release` command, even on windows, but it requires a lot of dependencies.

## Get the source

Get the source

```sh
git clone https://github.com/fermiumlabs/data-chan.git && cd data-chan
git submodule update --init --recursive #fetch the LUFA submodule
```

Between those two commands you can checkout another branch:

```sh
git checkout develop # before fetching LUFA switch to the unstable branch
```

Remember to checkout the new branch *before* updating the submodules.

## Building

With a build environment ready you can compile the source code with:

```sh
cd data-chan     # into the repo directory
make release -j2 # j2 flag is used to speed-up compilation time
```

Setting up a building environment can be a bit tedious.

## Environment

On a generic system you will need at least:

* make
* gcc-avr (somewhere called avr-gcc)
* avr-libc
* gcc
* g++
* libusb-1.0-0-dev
* libpthread-stubs0-dev (pre-installed on most systems)
* npm
* python      (^2.7)
* python-dev  (^2.7)

__NOTE:__ gcc and avr-libc used versions are 4.8, but any other greater versions should do a better job (gcc 6 compatibility is almost completely tested).

On Ubuntu/Debian run:

```sh
sudo apt install make gcc g++ libusb-1.0-0-dev gcc-avr avr-libc binutils-avr pyhon python-dev
```

On MacOS if you use Homebrew you have to tap an external repo:

```sh
brew tap osx-cross/avr
brew install avr-libc avr-gcc libusb
```

__NOTE:__ On MacOS if you want to build the node.js plugin you __have to__ install XCode!

The other way is __manually__ excluding node.js from the build and use the GNU C/C++ Compiler:

```sh
brew install make gcc g++
```

If you're using windows you will need to install:

* [msys 1.0](http://downloads.sourceforge.net/mingw/MSYS-1.0.11.exe)
* [tdm-gcc](http://tdm-gcc.tdragon.net/) - libusb-1.0-0-dev, libpthread-stubs0-dev
* [grep for windows](http://www.wingrep.com/)

If you are planning to use the Atmega32u4 or any other AVR microcontroller you will need an [USBASP](http://www.fischl.de/usbasp/) to program the MCU and [AVRDUDE](http://www.nongnu.org/avrdude/) to drive it.

__Note:__ You'll be able to use the firmware bootloader shipped with your device only once: read the [faq](faq.md)!

## Vagrant Environment

For your convenience a Vagrant development environment is provided.
It's based on Ubuntu Xenial and preconfigured with everything you need to develop on linux.

1. Install [Virtualbox](https://www.virtualbox.org/wiki/Downloads)
1. Install [Vagrant](https://www.vagrantup.com/downloads.html):
1. [Checkout](#get-the-source) the source code
1. Start Vagrant

```sh
vagrant up
vagrant ssh
```

At this point you are ready [build](#building) data-chan:

```sh
cd data-chan #symbolic link to /vagrant
make release -j2
```

## Embeddable library

You can build a library that can be embedded into any other (AVR) project:


```sh
make lib -j2
```
