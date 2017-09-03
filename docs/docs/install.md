# Install (development only)

You can install the compilation result (obtained from the [build](build.md) process) by tiping:

```sh
make install
```

However, be notified that it requires *root* privileges.

That command is mostly useful in a development PC, because:
- it adds the libDataChan where the system expects it to be found
- it generates and installs the documentation for your specific version
- it uploads the firmware to the MCU

# Install (Bundled with an application)

Data chan requires [libusb](http://libusb.info/).

### Linux

On Debian/ubuntu systems, require the [libusb-dev](https://packages.debian.org/it/wheezy/libusb-dev) package as a dependency. On Fedora/RedHat, the [LibUSB](https://admin.fedoraproject.org/pkgdb/package/rpms/libusb/) package.

### Windows

Data-chan requires the [Windows-compatible Pthread](http://sourceware.org/pthreads-win32/) library (dll) that when building with MinGW can be found in  `C:\tools\mingw64\opt\bin\libwinpthread-1.dll`

You should place the DLL either in the same folder as the application that uses libDataChan.dll or `C:\Windows\System32\libwinpthread-1.dll`

There's no need to add any LibUSB dll, since LibUSB is statically linked on Windows systems inside libDataChan.dll.
Nontheless, LibUSB requires a driver to rely on, which is not installed by default on Windows. For this reason you need to use a tool like [Zadig](http://zadig.akeo.ie/) to generate and install correctly a driver, such as WinUSB. More info can be found in the [LibUSB Wiki](https://github.com/libusb/libusb/wiki/Windows)

### MacOS

Data chan will look for LibUSB on `@rpath/libusb/libusb.dylib`. What's `@rpath` ? take a look [here](https://en.wikipedia.org/wiki/Rpath), [here](https://stackoverflow.com/questions/33991581/install-name-tool-to-update-a-executable-to-search-for-dylib-in-mac-os-x), [here](https://wincent.com/wiki/%40executable_path%2C_%40load_path_and_%40rpath). 

This means that inside your app bundle, libusb.dylib should be found at the following path `MyApp.app/Contents/Frameworks/libusb/libusb.dylib`

Please note that this is true only for binaries downloaded from this repo (built from Travis-CI) and not for binaries normally built. To fix the library with the relative path you should to run the following commands:

```shell
otool -L **/libDataChan.dylib
install_name_tool -change "/usr/local/opt/libusb/lib/libusb-1.0.0.dylib" "@rpath/libusb/libusb.dylib" **/libDataChan.dylib
otool -L **/libDataChan.dylib
```
