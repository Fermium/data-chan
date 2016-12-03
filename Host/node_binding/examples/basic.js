var datachan_lib = require('../build/Release/DataChan');

// you don't want an exception each time you open a device, right?
datachan_lib.DataChan.Init();

try {
    // what a good time to open a new device :)
    var f = new datachan_lib.Device();
} catch (ex) {
    console.error('error opening the device');
}

// it is important to call this
datachan_lib.DataChan.Shutdown();