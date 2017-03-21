var datachan = require('../data-chan.js');
var datachan_lib = datachan.lib;
var datachan_result_enum = datachan.search_enum;

// you don't want an exception each time you open a device, right?
datachan_lib.datachan_init();

// what a good time to open a new device :)
var scan_result = datachan_lib.datachan_device_acquire();
if (scan_result.result == datachan_result_enum.success) {
    console.log('Device opened!');

	// this is the device you acquired
    var dev = scan_result.device;

	// release the device
    datachan_lib.datachan_device_release(dev);
} else {
    console.log('Error opening the device: ');
    console.log(scan_result.result);
}

// it is important to call this
datachan_lib.datachan_shutdown();
