# Install

You can install the compilation result (obtained from the [build](build.md) process) by tiping:

```sh
make install
```

However, be notified that it requires *root* privileges.

That command is also useful in a development PC, because:
- it adds the libDataChan where the system expects it to be found
- it generates and installs the documentation for your specific version
- it uploads the firmware to the MCU
