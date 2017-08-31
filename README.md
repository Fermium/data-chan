# Data-chan

| Linux | Linux | Linux and macOS | Windows |
| :---: | :---: | :---: | :---: |
| Host, Device and docs | HW (Host, Device, Flashing) | Host only | Host only |
| Deploy docs | No deployment | Deploy lib | Deploy lib|
 | [![wercker status](https://app.wercker.com/status/75da37fe4a9fc37d604da68759866454/s/master "wercker status")](https://app.wercker.com/project/byKey/75da37fe4a9fc37d604da68759866454) | [![Build Status](https://drone-ci.dev.fermiumlabs.com/api/badges/fermiumlabs/data-chan/status.svg)](https://drone-ci.dev.fermiumlabs.com/fermiumlabs/data-chan) | [![Build Status](https://travis-ci.org/fermiumlabs/data-chan.svg?branch=master)](https://travis-ci.org/fermiumlabs/data-chan)  | [![Build status](https://ci.appveyor.com/api/projects/status/a8svd5ij2f9mdh7m?svg=true)](https://ci.appveyor.com/project/dfermiumlabs/data-chan) |
 

Data-chan is an USB framework/protocol for communication between Atmel AVR devices (mainly Atmega 32u4) and a PC.

At the current time it can transfer over 800 measures every second.

It makes usage of [LUFA](http://www.fourwalledcubicle.com/LUFA.php) on the device side and [Libusb](http://www.libusb.org/) on the host.

The framework is though for physics instruments with a slow sample rate, and allow to receive measures and send commands.

It's thought to be:

* Minimal
* Fast
* Configurable

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

## Download

The compiled libraries can be downloaded in [this](https://data-chan-js-binaries.s3.amazonaws.com/index.html) s3 bucket, identified by the commit hash. 
