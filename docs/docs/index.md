# Data-Chan

Data-chan is either a protocol and a collection of subroutines to use the protocol.

Data-chan is used to send physics measures from a generic device
to the PC using native USB, __NOT AN UGLY SERIAL->USB IC!__

Due to the diversity of electronics devices and needs the protocol is designed to be:

     * simple and minimal
	 * fast
     * costomizable
	 
## Atmel AVR

On this repository you can find the device client program for the atmega32u4 and other AVR microcontrollers.

Have you prepared your arduino nano pro?


## Measures and Commands

Receiving measures from a generic USB device is the main goal a data-chan.

We all know that physics devices needs command and configuration to properly operate,
to fulfill this need data-chan can send two types of commands to the target device:

     * protocol-specific commands
	 * free commands