# Build

The entire project can be compiled with the `make debug` or `make release` command, even on windows, but it requires a lot of dependencies.

## Get the source

Get the source

```sh
git clone https://github.com/NeroReflex/data-chan.git && cd data-chan
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
* python      (^2.7)
* python-dev  (^2.7)

__NOTE:__ gcc and avr-libc used versions are 4.X, but any other greater versions should do a better job (gcc 6 compatibility is almost completely tested).

On Ubuntu/Debian run:

```sh
sudo apt install make gcc g++ libusb-1.0-0-dev gcc-avr avr-libc binutils-avr pyhon python-dev
```

On MacOS if you use Homebrew you have to tap an external repo:

```sh
brew tap osx-cross/avr
brew install avr-libc49 avr-gcc49 libusb
```

For now compatibility with avr-gcc 7 (avr-gcc in brew) appears to be broken

If you're using windows you will need to install:

* [msys 1.0](http://downloads.sourceforge.net/mingw/MSYS-1.0.11.exe)
* [tdm-gcc](http://tdm-gcc.tdragon.net/) - libusb-1.0-0-dev, libpthread-stubs0-dev
* [grep for windows](http://www.wingrep.com/)

If you are planning to use the Atmega32u4 or any other AVR microcontroller you will need an [USBASP](http://www.fischl.de/usbasp/) to program the MCU and [AVRDUDE](http://www.nongnu.org/avrdude/) to drive it.

__Note:__ You'll be able to use the firmware bootloader shipped with your device only once: read the [faq](faq.md)!

## Vagrant Environment

For your convenience three Vagrant development environments are provided.

* "arch", based on [Arch Linux](https://www.archlinux.org/), for bleeding edge development
* "ubuntu", based on Ubuntu Xenial, for stable development
* "windows", based on windows server. It is required access to a working Windows server 2012/2016 vagrant box from [boxcutter/windows](https://github.com/boxcutter/windows) ( windows server 2016 supported [here](https://github.com/fermiumlabs/boxcutter-windows) )

```shell
cd .. #get outside of the data-chan repo
git clone git@github.com:boxcutter/windows.git
make virtualbox/eval-win2016-standard-ssh
vagrant box add box/virtualbox/eval-win2012r2-standard-ssh-*.box --name eval-win2016-standard-ssh
cd data-chan 
vagrant up windows
vagrant reload #to reload powershell
```


1. Install [Virtualbox](https://www.virtualbox.org/wiki/Downloads)
1. Install [Vagrant](https://www.vagrantup.com/downloads.html):
1. [Checkout](#get-the-source) the source code
1. Start Vagrant

```sh
vagrant up ubuntu
vagrant ssh ubuntu
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
