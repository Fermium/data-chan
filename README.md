# Data-chan

| Linux | Linux | Linux and macOS | Windows |
| :---: | :---: | :---: | :---: |
| Host, Device and docs | HW (Host, Device, Flashing) | Host only | Host only |
| Deploy docs | No deployment | Deploy lib | Deploy lib|
 | [![wercker status](https://app.wercker.com/status/bdfa5177fa0f6ce71d32e1ba1f257127/s/master "wercker status")](https://app.wercker.com/project/byKey/bdfa5177fa0f6ce71d32e1ba1f257127) | [![Build Status](https://drone-ci.dev.fermiumlabs.com/api/badges/NeroReflex/data-chan/status.svg)](https://drone-ci.dev.fermiumlabs.com/NeroReflex/data-chan) | [![Build Status](https://travis-ci.org/NeroReflex/data-chan.svg?branch=master)](https://travis-ci.org/NeroReflex/data-chan)  | [![Build status](https://ci.appveyor.com/api/projects/status/gwy7av54b9j17oa3?svg=true)](https://ci.appveyor.com/project/NeroReflex/data-chan) |
 

Data-chan is an USB framework/protocol for communication between Atmel AVR devices (mainly Atmega 32u4) and a PC.

At the current time it can transfer over 800 measures every second.

It makes usage of [LUFA](http://www.fourwalledcubicle.com/LUFA.php) on the device side and [Libusb](http://www.libusb.org/) on the host.

The framework is though for physics instruments with a slow sample rate, and allow to receive measures and send commands.

It's thought to be:

* Minimal
* Fast
* Configurable

## Docs
Documentation is kept up-to-date with the master branch.
Check out the [DOCS](https://neroreflex.github.io/data-chan/) to learn more!

## Features:

* Broad compatibility with MacOS/Windows/Linux
* Bindings for most of the major programming languages
* Do not require user interaction to install the required drivers
* Small resource footprint on the MCU
* Portable to other devices compatible with the LUFA library (and it's forks)

## Compatibility

The Host API are compatible with (almost) any operating system, because they are
based on pthread and Libusb.

Official compatibility list is:

* Windows XP or later
* Linux kernel 2.6 or later
* FreeBSD 9.0 or later
* MacOS 10.8 or later

On any other system that supports libusb1.0 you may need to install a generic Libusb driver to have your device working.

# Libraries

Libraries are available to interface data-chan with various programming languages:

* NodeJS: [fermiumlabs/data-chan-nodejs](https://github.com/fermiumlabs/data-chan-nodejs)
* Python: [fermiumlabs/data-chan-python](https://github.com/fermiumlabs/data-chan-python)

## Download

The compiled libraries can be downloaded in [this](https://data-chan-libs.s3.amazonaws.com/index.html) s3 bucket, identified by the commit hash.
