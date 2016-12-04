var datachan_lib = require('../build/Release/DataChan');

// you don't want an exception each time you open a device, right?
datachan_lib.datachan_init();

// what a good time to open a new device :)
var scan_result = datachan_lib.datachan_device_acquire();
if (scan_result.result == datachan_lib.success) {
    console.log('Device opened!');
} else {
    console.log('Error opening the device: ');
	console.log(scan_result.result);
}

// it is important to call this
datachan_lib.datachan_shutdown();
