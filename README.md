# Data-chan

[![Build Status](https://travis-ci.org/NeroReflex/data-chan.svg?branch=master)](https://travis-ci.org/NeroReflex/data-chan)

[![wercker status](https://app.wercker.com/status/bdfa5177fa0f6ce71d32e1ba1f257127/s/master "wercker status")](https://app.wercker.com/project/byKey/bdfa5177fa0f6ce71d32e1ba1f257127)

Data-chan is an USB framework/protocol for communication between Atmel AVR devices (mainly Atmega 32u4) and a PC.

At the current time it can transfer over 800 measures every second.

It makes usage of [LUFA](http://www.fourwalledcubicle.com/LUFA.php) on the device side and [Libusb](http://www.libusb.org/) on the host.

The framework is though for physics instruments with a slow sample rate, and allow to receive measures and send commands.

It's thought to be:

* Minimal
* Fast
* Configurable

##Features:

* Broad compatibility with MacOS/Windows/Linux
* Bindings for most of the major programming languages
* Do not require user interaction to install the required drivers
* Small resource footprint on the MCU
* Portable to other devices compatible with the LUFA library (and it's forks)

## Compatibility

The Host API are compatible with (almost) any operating system, because they are
based on pthread and libusb.

Official compatibility list is:

* Windows XP or later
* Linux kernel 2.6 or later
* FreeBSD 9.0 or later
* MacOS 10.8 or later

On any other system that supports libusb1.0 you may need to install a generic libusb driver to have your device working.

## Docs
Documentation is kept up-to-date with the master branch.
Check out the [DOCS](https://neroreflex.github.io/data-chan/) to learn more!
