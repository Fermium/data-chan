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

int datachan_raw_read(datachan_device_t* dev, uint8_t* data) {
    if ((dev == (datachan_device_t*)NULL) || (data == (uint8_t*)NULL))
        return 0;

    int bytes_transferred = 0, result = 0;;

    // create a private safe buffer
    uint8_t data_in[GENERIC_REPORT_SIZE];

    // perform the data transmission
    pthread_mutex_lock(&dev->handler_mutex);
    result = libusb_bulk_transfer(
            dev->handler,
            USB_IN_ENDPOINT,
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

    // append the CRC byte for error catching purpose
    data_out[GENERIC_REPORT_SIZE - 1] = CRC_calc(data_out, GENERIC_REPORT_SIZE - 1);

    // perform the data transmission
    pthread_mutex_lock(&dev->handler_mutex);
    result = libusb_bulk_transfer(
            dev->handler,
            USB_OUT_ENDPOINT,
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

void* IO_bulk_thread(void* device) {
    if (device == NULL)
        pthread_exit(NULL);

    datachan_device_t* dev = (datachan_device_t*)device;
    uint8_t data_in[GENERIC_REPORT_SIZE];
    int data_size;
    measure_t m;

    while (datachan_device_is_enabled(dev)) {
        // get the bulk to be written
        uint8_t out_buffer[GENERIC_REPORT_SIZE - 1];
        datachan_dequeue_request(dev, out_buffer);
        
        // write to the device
        data_size = datachan_raw_write(dev, out_buffer, GENERIC_REPORT_SIZE - 1);
        
        // read from the device
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
