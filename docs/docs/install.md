# Install

You can install the compilation result (obtained from the [build](build.md) process) by tiping:

```sh
make install
```

However, be notified that it requires *root* privileges.

That command is mostly useful in a development PC, because:
- it adds the libDataChan where the system expects it to be found
- it generates and installs the documentation for your specific version
- it uploads the firmware to the MCU

# Deployment in applications

Data chan requires [libusb](http://libusb.info/).

### Linux

On Debian/ubuntu systems, require the [libusb-dev](https://packages.debian.org/it/wheezy/libusb-dev) package as a dependency. On Fedora/RedHat, the [libusb](https://admin.fedoraproject.org/pkgdb/package/rpms/libusb/) package.

### Windows

Place the libusb dll inside directory from which the application loaded.

### MacOS

Data chan will look for libusb on `@rpath/libusb/libusb.dylib`. What's `@rpath` ? take a look [here](https://en.wikipedia.org/wiki/Rpath), [here](https://stackoverflow.com/questions/33991581/install-name-tool-to-update-a-executable-to-search-for-dylib-in-mac-os-x), [here](https://wincent.com/wiki/%40executable_path%2C_%40load_path_and_%40rpath). 

This means that inside your app bundle, libusb.dylib should be found at the following path `MyApp.app/Contents/Frameworks/libusb/libusb.dylib`

Please note that this is true only for binaries downloaded from this repo (built from Travis-CI) and not for binaries normally built. To fix the library with the relative path you should to run the following commands:

```shell
otool -L **/libDataChan.dylib
install_name_tool -change "/usr/local/opt/libusb/lib/libusb-1.0.0.dylib" "@rpath/libusb/libusb.dylib" **/libDataChan.dylib
otool -L **/libDataChan.dylib
```
