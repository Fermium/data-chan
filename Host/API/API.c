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

#include "../../config.h"
#include "API.h"
#include "../../Protocol/measure_functions.h"
#include "../../Protocol/data_management_functions.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>

#define USB_USED_INTERFACE 0
#define INTERRUPT_IN_ENDPOINT 0x81
#define INTERRUPT_OUT_ENDPOINT 0x02
#define TIMEOUT_MS 1000

static libusb_context* ctx = (libusb_context*)NULL;

/********************************************************************************
 *                          repack measure and check the CRC                    *
 ********************************************************************************/

void repack_measure(measure_t* out, uint8_t* in) {
    // the first byte is the type of measure sent
    memcpy((void*)&out->type, (const void*)in, sizeof(out->type));
    in += sizeof(out->type);

    // the second byte is the source channel
    memcpy((void*)&out->channel, (const void*)in, sizeof(out->channel));
    in += sizeof(out->channel);

    // let's continue with the value...
    memcpy((void*)&out->value, (const void*)in, sizeof(out->value));
    in += sizeof(out->value);

    // ...measurement unit...
    memcpy((void*)&out->mu, (const void*)in, sizeof(out->mu));
    in += sizeof(out->mu);

    // ..time...
    memcpy((void*)&out->time, (const void*)in, sizeof(out->time));
    in += sizeof(out->time);

    // ..millis...
    memcpy((void*)&out->millis, (const void*)in, sizeof(out->millis));
    in += sizeof(out->millis);
}

/********************************************************************************
 *              Device Structure initializer and deinitializer                  *
 ********************************************************************************/

datachan_device_t* datachan_device_setup(libusb_device_handle* native_handle) {
    // allocate memory for the device
    datachan_device_t* dev = (datachan_device_t*)malloc(sizeof(datachan_device_t));

    // register the native handle
    dev->handler = native_handle;

    // prepare the internal queue
    dev->measures_queue.first = (struct fifo_queue_t *)NULL;
    dev->measures_queue.last = (struct fifo_queue_t *)NULL;
    dev->measures_queue.count = 0;

    // the device is disabled, there is nothing to read, and a reader thread is unnecessary
    dev->enabled = false;

    // pthread attribute creation
    pthread_attr_init(&dev->reader_attr);

    pthread_mutexattr_init(&dev->mutex_attr);

    // pthread mutex
    pthread_mutex_init(&dev->measures_queue_mutex, &dev->mutex_attr);
    pthread_mutex_init(&dev->enabled_mutex, &dev->mutex_attr);
    pthread_mutex_init(&dev->handler_mutex, &dev->mutex_attr);

    // enjoy the device
    return dev;
}

void datachan_device_cleanup(datachan_device_t* dev) {
    // no need for the thread
    pthread_attr_destroy(&dev->reader_attr);

    // remove the mutex safely (acquire and release it first)
    pthread_mutex_destroy(&dev->enabled_mutex);
    pthread_mutex_destroy(&dev->measures_queue_mutex);
    pthread_mutex_destroy(&dev->handler_mutex);
	
    // remove the mutex attribute safely
    pthread_mutexattr_destroy(&dev->mutex_attr);
}

/********************************************************************************
 *				message reader					*
 ********************************************************************************/

void* msg_reader_thread(void* device) {
    if (device == NULL)
        pthread_exit(NULL);
    
    datachan_device_t* dev = (datachan_device_t*)device;
    uint8_t data_in[GENERIC_REPORT_SIZE];
    int data_size;
    measure_t m;

    while (datachan_device_is_enabled(dev)) {
        data_size = datachan_raw_read((datachan_device_t*)dev, data_in);

        // deserialize the received measure only if it really is a valid measure
        if ((data_size > 0) && (*(data_in) == MEASURE)) {
            // deserialize the measure and insert the result into the queue
            repack_measure(&m, data_in + 1);
            datachan_device_enqueue_measure(dev, (const measure_t*)&m);
        }
    }

    return NULL;
} 
 
/********************************************************************************
 *					Measures Queue				*
 ********************************************************************************/
 
void datachan_device_enqueue_measure(datachan_device_t* dev, const measure_t* m) {
    if ((dev == (datachan_device_t*)NULL) || (m == (const measure_t*)NULL))
        return;
    
    // copy the measure in a safe place
    measure_t* measure_copy = (measure_t*)malloc(sizeof(measure_t));
    memcpy((void*)measure_copy, (const void*)m, sizeof(measure_t));

    if (measure_copy->type == NONREALTIME) {
        long            ms; // Milliseconds
        time_t          s;  // Seconds
        struct timespec spec;

        // get the UNIX time and millis
        clock_gettime(CLOCK_REALTIME, &spec);
        s  = spec.tv_sec;
        ms = round(spec.tv_nsec / 1.0e6);
        
        measure_copy->time = s;
        measure_copy->millis = ms;
    }
    
    // lock on the queue and perform the insertion
    pthread_mutex_lock(&dev->measures_queue_mutex);
    enqueue_measure(&dev->measures_queue, measure_copy);
    pthread_mutex_unlock(&dev->measures_queue_mutex);
}

measure_t* datachan_device_dequeue_measure(datachan_device_t* dev) {
    if (dev == (datachan_device_t*)NULL)
        return (measure_t*)NULL;
        
    // the measure
    measure_t* measure = (measure_t*)NULL;
    
    // lock on the queue and perform the deletion
    pthread_mutex_lock(&dev->measures_queue_mutex);
    measure = dequeue_measure(&dev->measures_queue);
    pthread_mutex_unlock(&dev->measures_queue_mutex);
    
    return measure;
}

int32_t datachan_device_enqueued_measures(datachan_device_t* dev) {
    if (dev == (datachan_device_t*)NULL)
        return -1;
    
    uint32_t count = 0;
    
    // lock on the queue and get the actual measures count
    pthread_mutex_lock(&dev->measures_queue_mutex);
    count = count_measures(&dev->measures_queue);
    pthread_mutex_unlock(&dev->measures_queue_mutex);
    
    return (int32_t)count;
}

/********************************************************************************
 *				Public Device API				*
 ********************************************************************************/

bool datachan_is_initialized() {
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

datachan_acquire_result_t datachan_device_acquire(void) {
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

void datachan_device_release(datachan_device_t** dev) {
    if ((dev == (datachan_device_t**)NULL) || (*dev == (datachan_device_t*)NULL))
        return;
    
    // make sure the device won't send precious data to the OS
    datachan_device_disable(*dev);
	
    // lock the device (wait for the last operation to finish and prevent usage of device))
    pthread_mutex_lock(&(**dev).handler_mutex);
    libusb_close((**dev).handler);

    // device structure clean
    datachan_device_cleanup(*dev);
	
    // avoid dangling pointer
    *dev = (datachan_device_t*)NULL;
}

bool datachan_device_set_config(datachan_device_t* dev, uint32_t entry, uint8_t channel, void* data, uint16_t data_size) {
    //this is the data buffer
    uint8_t cmd[GENERIC_REPORT_SIZE] = { CMD_REQUEST, SET_CONFIG_FLAG };
    uint8_t *cmd_builder_buffer = cmd + 2;
    
    // fill unused space with zeroes
    memset(cmd_builder_buffer, 0, GENERIC_REPORT_SIZE - 2);
    
    // write the setting ID
    memcpy((void*)cmd_builder_buffer, (const void*)&entry, sizeof(entry));
    cmd_builder_buffer += sizeof(entry);
    
    // write the destination channel
    memcpy((void*)cmd_builder_buffer, (const void*)&channel, sizeof(channel));
    cmd_builder_buffer += sizeof(channel);
    
    // prevent buffer overflow (12 means: CMD_REQUEST(1), SET_CONFIG_FLAG(1), entry(4), channel(1), data_size(2), CRC(1) )
    data_size = (data_size > (GENERIC_REPORT_SIZE - 10)) ? GENERIC_REPORT_SIZE - 10 : data_size;
    
    // write the data length
    memcpy((void*)cmd_builder_buffer, (const void*)data_size, sizeof(data_size));
    cmd_builder_buffer += sizeof(data_size);
    
    // write the data
    memcpy((void*)cmd_builder_buffer, (const void*)data, data_size);
    
    // success?
    return (datachan_raw_write(dev, cmd, sizeof(cmd)) == GENERIC_REPORT_SIZE);
}

int datachan_raw_read(datachan_device_t* dev, uint8_t* data) {
    if ((dev == (datachan_device_t*)NULL) || (data == (uint8_t*)NULL))
        return 0;
    
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
    
    // check for the CRC
    if ((bytes_transferred == GENERIC_REPORT_SIZE) && (!CRC_check(data_in, GENERIC_REPORT_SIZE - 1, data_in[GENERIC_REPORT_SIZE - 1])))
        bytes_transferred = 0;
    
    // copy the result on the unsafe buffer (on success)
    if ((result == 0) && (bytes_transferred > 0))	
        memcpy((void*)data, (const void*)data_in, bytes_transferred - 1);
    else if (bytes_transferred != 0)
        bytes_transferred = 0;

    return bytes_transferred;
}

int datachan_raw_write(datachan_device_t* dev, uint8_t* data, int data_length) {
    if ((dev == (datachan_device_t*)NULL) || (data == (uint8_t*)NULL) || (data_length == 0))
        return 0;
    
    int bytes_transferred = 0, result = 0;

    // avoid buffer overflow during memcpy
    data_length = (data_length > (GENERIC_REPORT_SIZE-1)) ? GENERIC_REPORT_SIZE-1 : data_length;
	
    // zero everything unused and copy the buffer
    uint8_t data_out[GENERIC_REPORT_SIZE];
    memset((void*)data_out, 0, sizeof(data_out));
    memcpy((void*)data_out, data, data_length);
    
    // append the CRC byte for error catching purpouse
    data_out[GENERIC_REPORT_SIZE - 1] = CRC_calc(data_out, GENERIC_REPORT_SIZE - 1);
    
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
    if (dev == (datachan_device_t*)NULL)
        return false;
    
    bool enabled;
	
    pthread_mutex_lock(&dev->enabled_mutex);
    enabled = dev->enabled;
    pthread_mutex_unlock(&dev->enabled_mutex);
	
    return enabled;
}

bool datachan_device_enable(datachan_device_t* dev) {
    if (dev == (datachan_device_t*)NULL)
        return false;
    
    bool enabled = datachan_device_is_enabled(dev);
	
    if (!enabled) {
        // generate the enable command
        uint8_t cmd[] = { CMD_REQUEST, ENABLE_TRANSMISSION };
		
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
    if (dev == (datachan_device_t*)NULL)
        return false;
    
    bool enabled = datachan_device_is_enabled(dev);
	
    if (enabled) {
        // generate the enable command
        uint8_t cmd[] = { CMD_REQUEST, DISABLE_TRANSMISSION };

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
