# Data-chan

Data chan is an USB framework/protocol for comunication between Atmel AVR devices (mainly Atmega 32u4) and a PC.

It makes usage of [lufa](http://www.fourwalledcubicle.com/LUFA.php) on the device side and [Libusb](http://www.libusb.org/) on the host.

The framework it's though for datalogging instruments with a slow sample rate, and allow to receive data and to send commands.

It's tought to be:

* Minimal
* Fast 
* Configurable

##Features:

* Broad compatibility with MacOS/Windows/Linux
* Bindings for most of the major programming languages
* Do not require user interaction to install the required drivers
* Small resource footprint on the MCU
* Portable to other devices compatible with the LUFA library (and it's forks)

## Docs

Check out the [DOCS](https://neroreflex.github.io/data-chan/) for more info.
