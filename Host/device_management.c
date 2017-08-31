/**
	data-chan physic through USB
	Copyright (C) 2017 Fermium LABS srl

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../config.h"
#include "DataChan.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>

// defined in lib_init.c
extern libusb_context* ctx;

datachan_acquire_result_t datachan_device_acquire(uint16_t vid, uint16_t pid) {
    datachan_acquire_result_t res;
    res.device = (datachan_device_t*)NULL;
    res.result = unknown;

    if (datachan_is_initialized()) {
        // search for the associated device
        libusb_device_handle* handle = libusb_open_device_with_vid_pid(ctx, vid, pid);

        if (handle != (libusb_device_handle*)NULL) {
            //libusb_set_auto_detach_kernel_driver(handle, 1);

            // setting the configuration 1 means selecting the corresponding bConfigurationValue
            if (libusb_claim_interface(handle, USB_USED_INTERFACE) == 0) {
                // fill the device structure
                res.device = datachan_device_setup(handle);
                res.result = success;
            } else res.result = cannot_claim;
        } else res.result = not_found_or_inaccessible;
    } else res.result = uninitialized;

    // report operation result
    return res;
}

void datachan_device_release(datachan_device_t* dev) {
    if ((dev == (datachan_device_t*)NULL) || (dev == (datachan_device_t*)NULL))
        return;

    // make sure the device won't send precious data to the OS
    datachan_device_disable(dev);

    // lock the device (wait for the last operation to finish and prevent usage of device))
    pthread_mutex_lock(&dev->handler_mutex);
    libusb_close(dev->handler);

    // device structure clean
    datachan_device_cleanup(dev);
}
