
# FAQ

#### In what way is the native USB in Data-chan better than a serial-USB IC?

Those ICs are expensive. The usage of the integrated USB hardware in an Atmega32u4 (or other compatible MCUs) it's more cost-effective.

Secondly, serial-USB ICs do not work that well. There have been many reported problems in the past such as the [FTDIgate](https://hackaday.com/tag/ftdi-gate/) and numerous driver signing problems in other [badly supported ICs](https://tzapu.com/making-ch340-ch341-serial-adapters-work-under-el-capitan-os-x/).

Thirdly, most of the time they require custom drivers that may not be available across the most common operative systems (and when they do often they are not kept up-to-date).

#### Why is the native USB Data-chan better than USB-CDC?

[USB-CDC](https://en.wikipedia.org/wiki/USB_communications_device_class) are most of the times claimed by the kernel on modern operating systems. If you use something as an RS232-over-CDC it will be claimed by the kernel and mapped to a serial device which will then need to be claimed by the application (requiring roots rights). It's messy.

Another reason is the connection speed: with an RS232-over-CDC the maximum data in/out speed is the maximum speed of the slower serial!

#### Should I use Data-chan for [insert project name here]?

Data-chan is a simple and well-defined protocol for a specific usage in data logging instruments, mostly in the field of physics.

If you can apply the same logic to your project, and you want to use low-cost devices while reaching high-speed performance... why not?

#### Which device can I use for testing and/or development?

An [Arduino Nano](https://www.arduino.cc/en/Main/ArduinoBoardNano) or [Arduino Leonardo](https://www.arduino.cc/en/main/arduinoBoardLeonardo) programmed with an [USBASP](http://www.fischl.de/usbasp/) and [AVRDUDE](http://www.nongnu.org/avrdude/) will do the job.

**WARNING:** Data-chan will break the bootloader shipped with the Arduino board!

#### What drivers does Data-chan use?

It uses [Winusb](https://msdn.microsoft.com/it-it/library/windows/hardware/ff540196(v=vs.85).aspx) on Windows.

In any other OS, generic vendor-specific class drivers, bundled with the OS are used.

#### How does Data-chan performs?

Using an ATmega32u4 with the shipped *Device* code you can transfer, each second, a number of IN packets that varies between 800 and  900.

Each packet is (by default) 64 bytes long and followed by an OUT packet.

With this configuration, you'll be able to transfer data up to 54kb/s in download and 54kb/s in upload!

Heck, with a sample rate of 200kHz (the fastest you can use with the integrated ADC in an Atmega32u4) you can only generate about 15kb of data each second!

The tests are conducted on MacOS 10.12 and Linux 4.7 kernel using the provided tester program.

#### I a beginner, should I use it?

Depends. It's not made to work with beginner-friendly boards as the Arduino UNO or the Arduino Duemilanove or with the Arduino software environment.

#### I have found a bug

[Submit an issue!](https://github.com/NeroReflex/data-chan/issues/new)
