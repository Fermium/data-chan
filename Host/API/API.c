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

/****************************************************
 *				message reader						*
 ****************************************************/

void* msg_reader_thread(void* device) {
	datachan_device_t* dev = (datachan_device_t*)device;
	uint8_t data_in[GENERIC_REPORT_SIZE];
	int data_size;
	measure_t m;
	
	while (datachan_device_is_enabled(dev)) {
		data_size = datachan_raw_read((datachan_device_t*)dev, data_in);
		
		// this is used as a data cursor
		uint8_t *data_ptr = data_in;
		
		// deserialize the received measure only if it really is a valid measure
		if ((data_size > 0) && (*(data_ptr++) == MEASURE)) {
			
			// deserialize and check if data really is valid
			if (repack_measure(&m, data_ptr) == REPACK_SUCCESS)
				datachan_device_enqueue_measure(dev, (const measure_t*)&m);
		}
	}
	
	return NULL;
} 
 
/********************************************************************************
 *					Measures Queue				*	*
 ********************************************************************************/
 
void datachan_device_enqueue_measure(datachan_device_t* dev, const measure_t* m) {
	// copy the measure in a safe place
	measure_t* measure_copy = (measure_t*)malloc(sizeof(measure_t));
	memcpy((void*)measure_copy, (const void*)m, sizeof(measure_t));
	
	// lock on the queue and perform the insertion
	pthread_mutex_lock(&dev->measures_queue_mutex);
	enqueue_measure(&dev->measures_queue, measure_copy);
	pthread_mutex_unlock(&dev->measures_queue_mutex);
}

measure_t* datachan_device_dequeue_measure(datachan_device_t* dev) {
    // the measure
    measure_t* measure = (measure_t*)NULL;
    
    // lock on the queue and perform the deletion
    pthread_mutex_lock(&dev->measures_queue_mutex);
    dequeue_measure(&dev->measures_queue);
    pthread_mutex_unlock(&dev->measures_queue_mutex);
    
    return measure;
}

uint32_t datachan_device_enqueue_measures(datachan_device_t* dev) {
    uint32_t count = 0;
    
    // lock on the queue and get the actual measures count
    pthread_mutex_lock(&dev->measures_queue_mutex);
    count = count_measures(&dev->measures_queue);
    pthread_mutex_unlock(&dev->measures_queue_mutex);
    
    return count;
}

/********************************************************************************
 *				Public Device API				*	*
 ********************************************************************************/

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
				res.device = datachan_device_setup(handle);
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
	
    // lock & release the device
    pthread_mutex_lock(&(**dev).handler_mutex);
    libusb_close((**dev).handler);
	
    // device structure clean
    datachan_device_cleanup(*dev);
	
    // avoid dangling pointer
    *dev = (datachan_device_t*)NULL;
}

int datachan_raw_read(datachan_device_t* dev, uint8_t* data) {
    int bytes_transferred = 0, result = 0;;

    // create a private safe buffer
    uint8_t data_in[GENERIC_REPORT_SIZE];
	
    // perform the data transmission
    pthread_mutex_lock(&dev->handler_mutex);
    result = libusb_interrupt_transfer(
            dev->handler,
            INTERRUPT_IN_ENDPOINT,
            data_in,
            sizeof(data_in),
            &bytes_transferred,
            TIMEOUT_MS
        );
    pthread_mutex_unlock(&dev->handler_mutex);
	
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
    pthread_mutex_lock(&dev->handler_mutex);
    result = libusb_interrupt_transfer(
            dev->handler,
            INTERRUPT_OUT_ENDPOINT,
            data_out,
            sizeof(data_out),
            &bytes_transferred,
            TIMEOUT_MS
        );
    pthread_mutex_unlock(&dev->handler_mutex);
	
    // error check
    bytes_transferred = (result == 0) ? bytes_transferred : 0;
	
    return bytes_transferred;
}

bool datachan_device_is_enabled(datachan_device_t* dev) {
    bool enabled;
	
    pthread_mutex_lock(&dev->enabled_mutex);
    enabled = dev->enabled;
    pthread_mutex_unlock(&dev->enabled_mutex);
	
    return enabled;
}

bool datachan_device_enable(datachan_device_t* dev) {
    bool enabled = datachan_device_is_enabled(dev);
	
    if (!enabled) {
        // generate the enable command
        uint8_t cmd[] = { CMD_MAGIC_FLAG, ENABLE_TRANSMISSION };
		
        // write the command on the USB bus
        int data_size = datachan_raw_write(dev, cmd, sizeof(cmd));
		
        // report the result
        enabled = (data_size >= sizeof(cmd)) && 
            (pthread_create(
                (pthread_t *)&dev->reader,
                (const pthread_attr_t *)&dev->reader_attr,
                &msg_reader_thread,
                (void*)dev
            ) == 0);
		
        // save the status
        pthread_mutex_lock(&dev->enabled_mutex);
        dev->enabled = enabled;
        pthread_mutex_unlock(&dev->enabled_mutex);
    }
	
    // is the device enabled now?
    return enabled;
}

bool datachan_device_disable(datachan_device_t* dev) {
    bool enabled = datachan_device_is_enabled(dev);
	
    if (enabled) {
        // generate the enable command
        uint8_t cmd[] = { CMD_MAGIC_FLAG, DISABLE_TRANSMISSION };

        // write the command on the USB bus and if the result is good the device will be disabled
        if (datachan_raw_write(dev, cmd, sizeof(cmd)) >= sizeof(cmd)) {
            // if this point is reached the communication will fall in few millis
            enabled = false;

            // the next call to is_enabled will fail (even on the thread)
            pthread_mutex_lock(&dev->enabled_mutex);
            dev->enabled = enabled;
            pthread_mutex_unlock(&dev->enabled_mutex);

            // so... let's just wait for the thread to gracefully stop
            void *data = NULL;
            pthread_join((pthread_t)dev->reader, &data);
        }
    }

    // is the device enabled now?
    return !enabled;
}