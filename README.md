# data-chan
An USB protocol used to send data acquired from physics instruments.

## Main points
The protocol is designed to be:
<ul>
    <li>minimal</li>
    <li>fast</li>
    <li>configurable</li>
</ul>

## Compatibility
The Host API are compatible with (almost) any operating system, because they are
based on pthread and libusb.

Official compatibility list is:
<ul>
    <li>Windows XP or later</li>
    <li>Linux kernel 2.6 or upper</li>
    <li>FreeBSD 9.0 or later</li>
    <li>MacOS 10.8 or later</li>
</ul>

On any other system that supports libusb1.0 you may need to install a generic libusb
driver to have your device working.

## Speed
At the current state the protocol, implemented in an ATmega32u4 MCU
(source code located under the Device directory) the protocol can transfer
more than 800 measures/s.

## Documentation
As the protocol must be simple and highly configurable, everything you'll ever
need is [documented](https://neroreflex.github.io/data-chan/)!