/**
	data-chan physic through USB
	Copyright (C) 2016  Benato Denis
	
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

#include "../../Protocol/measure.h"
#include "../../config.h"
#include "API.h"

#include <libusb-1.0/libusb.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>

#define USB_USED_INTERFACE 0

static libusb_context* ctx = (libusb_context*)NULL;

int repack_measure(measure_t* out, uint8_t* in) {
    //return 0; // error found (using check algorithm)
    return 1;
}

int datachan_is_initialized() {
    return (ctx != (libusb_context*)NULL);
}

void datachan_init() {
    // no double init
    if (!datachan_is_initialized()) {
		// init libusb
		if (libusb_init(&ctx) == 0)  {
			if (datachan_is_initialized())
				libusb_set_debug(ctx, LIBUSB_LOG_LEVEL_INFO);
		}
	}
}

datachan_acquire_result_t acquire_device(void) {
	datachan_acquire_result_t res;
	res.device = (datachan_device_t*)NULL;
	res.result = unknown;
	
	// update the list of devices
	//libusb_find_busses();
    //libusb_find_devices();
	
	#ifdef LINUX
		char dname[50];
	#endif
	
	if (datachan_is_initialized()) {
		// search for the associated device
		libusb_device_handle* handle = libusb_open_device_with_vid_pid(ctx, USB_VID, USB_PID);
		
		if (handle != (libusb_device_handle*)NULL) {
			// create the device structure
			datachan_device_t *dev = (datachan_device_t*)malloc(sizeof(datachan_device_t));
			if (dev != (datachan_device_t*)NULL) {
				libusb_set_auto_detach_kernel_driver(handle, 1);
				
				// setting the configuration 1 means selecting the corresponding bConfigurationValue
				if (libusb_claim_interface(handle, USB_USED_INTERFACE) == 0) {
					// fill the device structure
					dev->handler = handle;
						
					// the device structure must be returned
					res.device = dev;
					res.result = success;
				} else res.result = cannot_claim;
			} else res.result = malloc_fail;
		} else res.result = not_found_or_inaccessible;
	} else res.result = uninitialized;
	
	// report operation result
	return res;
}
