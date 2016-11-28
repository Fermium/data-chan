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
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>

#if defined(__MACH__)
	#include <malloc/malloc.h>
#else
    #include <malloc.h>
#endif

static libusb_context* ctx = (libusb_context*)NULL;

bool datachan_is_initialized() {
    return (ctx != (libusb_context*)NULL);
}

void datachan_init() {
    // no double init
    if (!datachan_is_initialized()) {
    // init libusb
    if (libusb_init(&ctx) == 0)  {
#ifdef DEBUG
        if (datachan_is_initialized())
            libusb_set_debug(ctx, LIBUSB_LOG_LEVEL_INFO);
#endif // DEBUG
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

void datachan_device_set_config(datachan_device_t* dev, uint32_t entry, uint8_t channel, void* data, uint16_t data_size) {
    //this is the data buffer
    uint8_t cmd[GENERIC_REPORT_SIZE-1] = { CMD_REQUEST, SET_CONFIG_FLAG };
    uint8_t *cmd_builder_buffer = cmd + 2;

    // fill unused space with zeroes (minus two because cmd_builder_buffer points to cmd[2])
    memset(cmd_builder_buffer, 0, sizeof(cmd) - 2);

    // write the setting ID
    memcpy((void*)cmd_builder_buffer, (const void*)&entry, sizeof(entry));
    cmd_builder_buffer += sizeof(entry);

    // write the destination channel
    memcpy((void*)cmd_builder_buffer, (const void*)&channel, sizeof(channel));
    cmd_builder_buffer += sizeof(channel);

    // prevent buffer overflow (10 means: CMD_REQUEST(1), SET_CONFIG_FLAG(1), entry(4), channel(1), data_size(2), CRC(1) )
    data_size = (data_size > (GENERIC_REPORT_SIZE - 10)) ? GENERIC_REPORT_SIZE - 10 : data_size;

    // write the data length
    memcpy((void*)cmd_builder_buffer, (const void*)&data_size, sizeof(data_size));
    cmd_builder_buffer += sizeof(data_size);

    // write the data
    memcpy((void*)cmd_builder_buffer, (const void*)data, data_size);

    // enqueue the config cmd
    datachan_enqueue_request(dev, cmd);
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

void* IO_bulk_thread(void* device);
bool datachan_device_enable(datachan_device_t* dev) {
    if (dev == (datachan_device_t*)NULL)
        return false;

    bool enabled = datachan_device_is_enabled(dev);

    if (!enabled) {
        // generate the enable command
        uint8_t cmd[] = { CMD_REQUEST, ENABLE_TRANSMISSION };

        // write the command on the USB bus
        datachan_enqueue_request(dev, cmd);

        // report the result
        enabled = (pthread_create(
                (pthread_t *)&dev->reader,
                (const pthread_attr_t *)&dev->reader_attr,
                &IO_bulk_thread,
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
        datachan_enqueue_request(dev, cmd);
        
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

    // is the device enabled now?
    return !enabled;
}
