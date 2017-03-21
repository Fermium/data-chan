# Arduino

You like arduino libraries, don't you?

To allow you to use Arduino in your projects a simple arduino project is included in data-chan:
you can find it under Device/Arduino directory.

That example is meant to be modified in your own repository clone, because it required a lot of
efforts to modify and configure correctly [Arduino-Makefile](https://github.com/sudar/Arduino-Makefile).

Please, note that:
- you cannot use the Arduino bootloader to upload programs
- you cannot use the Serial over USB module
- you are __NOT__ going to modify loop() and setup() as in any other arduino environment

The arduino bootstrap project is meant to be used __EXACTLY__ as the datachan bootstrap project,
it just allows arduino-specific libraries to be used!


## Build & Upload

To generate the arduino bootstrap project you have to:

```sh
export IDE_DIR=<arduino_directory>
cd data-chan/Device
make arduino
```

This will generate a .hex file that you can upload to arduino using an usbasp:

```sh
make arduinoupload
```
