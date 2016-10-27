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
//#define INTERRUPT_IN_ENDPOINT 0x02
#define INTERRUPT_IN_ENDPOINT 0x81
#define INTERRUPT_OUT_ENDPOINT 0x02
#define TIMEOUT_MS 1000

static libusb_context* ctx = (libusb_context*)NULL;

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

void datachan_shutdown(void) {
	// usb shutdown
	libusb_exit(ctx);
	
	// avoid dangling pointer
	ctx = (libusb_context*)NULL;
}

datachan_acquire_result_t device_acquire(void) {
	datachan_acquire_result_t res;
	res.device = (datachan_device_t*)NULL;
	res.result = unknown;
	
	if (datachan_is_initialized()) {
		// search for the associated device
		libusb_device_handle* handle = libusb_open_device_with_vid_pid(ctx, USB_VID, USB_PID);
		
		if (handle != (libusb_device_handle*)NULL) {
			libusb_set_auto_detach_kernel_driver(handle, 1);
			
			// setting the configuration 1 means selecting the corresponding bConfigurationValue
			if (libusb_claim_interface(handle, USB_USED_INTERFACE) == 0) {
				
				// fill the device structure
				res.device = new_datachan_device_t((void*)handle);
				res.result = success;
			} else res.result = cannot_claim;
		} else res.result = not_found_or_inaccessible;
	} else res.result = uninitialized;
	
	// report operation result
	return res;
}

void device_release(datachan_device_t** dev) {
	// make sure the device won't send precious data to the OS
	datachan_device_disable(*dev);
	
	// release the device
	libusb_close((**dev).handler);
	
	// avoid dangling pointer
	*dev = (datachan_device_t*)NULL;
}

int datachan_raw_read(datachan_device_t* dev, uint8_t* data) {
	int bytes_transferred = 0, result = 0;;

	// create a private safe buffer
 	uint8_t data_in[GENERIC_REPORT_SIZE];
	
	// perform the data transmission
	result = libusb_interrupt_transfer(
				dev->handler,
				INTERRUPT_IN_ENDPOINT,
				data_in,
				sizeof(data_in),
				&bytes_transferred,
				TIMEOUT_MS
			);
	
	// copy the result on the unsafe buffer (on success)
	if ((result == 0) && (bytes_transferred > 0))	
		memcpy((void*)data, (const void*)data_in, bytes_transferred);
	else if (bytes_transferred != 0)
		bytes_transferred = 0;
	
	return bytes_transferred;
}

int datachan_raw_write(datachan_device_t* dev, uint8_t* data, int data_length) {
	int bytes_transferred = 0, result = 0;
	
	// avoid buffer overflow during memcpy
	data_length = (data_length > GENERIC_REPORT_SIZE) ? GENERIC_REPORT_SIZE : data_length;
	
	// zero everything unused and copy the buffer
	uint8_t data_out[GENERIC_REPORT_SIZE];
	memset((void*)data_out, 0, sizeof(data_out));
	memcpy((void*)data_out, data, data_length);

	// perform the data transmission
	result = libusb_interrupt_transfer(
				dev->handler,
				INTERRUPT_OUT_ENDPOINT,
				data_out,
				sizeof(data_out),
				&bytes_transferred,
				TIMEOUT_MS
			);
	
	// erro check
	bytes_transferred = (result == 0) ? bytes_transferred : 0;
	
	return bytes_transferred;
}

int datachan_device_enable(datachan_device_t* dev) {
	if (!dev->enabled) {
		// generate the enable command
		uint8_t cmd[] = { CMD_MAGIC_FLAG, ENABLE_TRANSMISSION };
		
		// write the command on the USB bus
		int data_size = datachan_raw_write(dev, cmd, sizeof(cmd));
	
		// report the result
		dev->enabled = (data_size >= sizeof(cmd));
	}
	
	// is the device enabled now?
	return dev->enabled;
}

int datachan_device_disable(datachan_device_t* dev) {
	if (dev->enabled) {
		// generate the enable command
		uint8_t cmd[] = { CMD_MAGIC_FLAG, DISABLE_TRANSMISSION };
		
		// write the command on the USB bus
		int data_size = datachan_raw_write(dev, cmd, sizeof(cmd));
	
		// report the result
		dev->enabled = !(data_size >= sizeof(cmd));
	}
	
	// is the device enabled now?
	return !dev->enabled;
}