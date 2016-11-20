
## FAQ

#### In what way is the native usb in Data-chan better than an serial-usb IC?

Those ICs are expensive. With Data-chan you can use the integrated hardware in an Atmega32u4 with is easily available. 
Second, they do not work that well. They require custom drivers most of the times, and there have been so many problems in the past such as the [FTDIgate](https://hackaday.com/tag/ftdi-gate/) and the many signing problems in other [badly supported ICs](https://tzapu.com/making-ch340-ch341-serial-adapters-work-under-el-capitan-os-x/).
Third, most of the time they require drivers, rarely available cross-platform.

#### Why is the native usb Data-chan better than USB-CDC?

[Usb-CDC](https://en.wikipedia.org/wiki/USB_communications_device_class) are most of the times claimed by the kernel on modern OS. If you use something as a RS232-over-CDC it will be claimed by the kernel and mapped to a serial device which will then need to be guessed. It's messy.

#### Should I use Data-chan for [insert project name here]? 

Data-chan is a well-defined protocol for a specific usage in datalogging instruments, mostly in the field of physics. If you can apply the same logic to your project, why not.

#### Which device can I use for testing?

And [Arduino Nano](https://www.arduino.cc/en/Main/ArduinoBoardNano) programmed with an [USBASP](http://www.fischl.de/usbasp/) and [AVRDUDE](http://www.nongnu.org/avrdude/) will do.

**WARNING:** Data-chan will break the bootloader!

#### What drivers does Data-chan use?

It use [Winusb](https://msdn.microsoft.com/it-it/library/windows/hardware/ff540196(v=vs.85).aspx) on Windows, and included generic vendor-specific class drivers on MacOS and Linux.

#### I a beginner, should I use it?

Depends. It's not made to work with the Arduino environment. 

#### I found a bug

[Submit an issue!](https://github.com/NeroReflex/data-chan/issues/new)