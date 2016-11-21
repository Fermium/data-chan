
# FAQ

#### In what way is the native usb in Data-chan better than an serial-usb IC?

Firstly, those ICs are expensive:
allowing you to use the integrated USB hardware in an Atmega32u4, and many others
MCU, will save moneys from additional components.

Secondly, they do not work that well:
there have been many problems in the past such as the [FTDIgate](https://hackaday.com/tag/ftdi-gate/) and many signing problems in other [badly supported ICs](https://tzapu.com/making-ch340-ch341-serial-adapters-work-under-el-capitan-os-x/).

Thirdly, most of the time they require custom drivers:
most of the times those drivers are not supporting many different Operating Systems,
and when they do, they are not kept up-to-date!

#### Why is the native usb Data-chan better than USB-CDC?

[USB-CDC](https://en.wikipedia.org/wiki/USB_communications_device_class) are most of the times claimed by the kernel on modern OSes. If you use something as a RS232-over-CDC it will be claimed by the kernel and mapped to a serial device which will then need to be claimed by the application. It's messy.

#### Should I use Data-chan for [insert project name here]?

Data-chan is a simple and well-defined protocol for a specific usage in datalogging instruments, mostly in the field of physics.

If you can apply the same logic to your project, and you want to use low-cost
and low-resources devices, while reaching high-speed performance... why not?

#### Which device can I use for testing and/or development?

An [Arduino Nano](https://www.arduino.cc/en/Main/ArduinoBoardNano) programmed with an [USBASP](http://www.fischl.de/usbasp/) and [AVRDUDE](http://www.nongnu.org/avrdude/) will do the job.

**WARNING:** Data-chan will break the bootloader shipped with the Arduino Nano!

#### What drivers does Data-chan use?

It uses [Winusb](https://msdn.microsoft.com/it-it/library/windows/hardware/ff540196(v=vs.85).aspx) on Windows.

In any other OS, generic vendor-specific class drivers, bundled with the OS are used.

#### How does Data-chan performs?

Using an ATmega32u4 with the shipped *Device* code you can transfer, each second,
a number of IN packets that vary between 800 and 860.

Since each packet is (by default) 64 bytes long, and every IN packet is followed
by an OUT packet you can transfer data up to 54kb/s in download and 54kb/s in upload!

Heck, with a sample rate of 200kHz (the fastest you can use within an ATmega32u4)
you can only generate about 15kb of data each second!

Tests are conducted with a MacOS 10.12 and Linux 4.7 kernel using the provided Tester
program.

#### I a beginner, should I use it?

Depends. It's not made to work with beginners-like boards as the Arduino UNO or the Arduino Duemilanove.

#### I have found a bug

[Submit an issue!](https://github.com/NeroReflex/data-chan/issues/new)
