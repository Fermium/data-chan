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

#include "../config.h"
#include "DataChan.h"

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


datachan_device_t* datachan_device_setup(libusb_device_handle* native_handle) {
    // allocate memory for the device
    datachan_device_t* dev = (datachan_device_t*)malloc(sizeof(datachan_device_t));

    // register the native handle
    dev->handler = native_handle;

    // prepare the internal queue
    dev->measures_queue.first = (struct fifo_queue_t *)NULL;
    dev->measures_queue.last = (struct fifo_queue_t *)NULL;
    dev->measures_queue.count = 0;

    // prepare the requests queue
    dev->requests_queue = (struct bulk_out_t*)NULL;
    
    // the device is disabled, there is nothing to read, and a reader thread is unnecessary
    dev->enabled = false;

    // pthread attribute creation
    pthread_attr_init(&dev->reader_attr);

    // start the command id counter from zero
    dev->async_used_id = 0;

    // initialize default mutex attributes
    pthread_mutexattr_init(&dev->mutex_attr);

    // pthread mutex
    pthread_mutex_init(&dev->measures_queue_mutex, &dev->mutex_attr);
    pthread_mutex_init(&dev->enabled_mutex, &dev->mutex_attr);
    pthread_mutex_init(&dev->handler_mutex, &dev->mutex_attr);
    pthread_mutex_init(&dev->requests_queue_mutex, &dev->mutex_attr);
    pthread_mutex_init(&dev->async_used_id_mutex, &dev->mutex_attr);

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
    pthread_mutex_destroy(&dev->requests_queue_mutex);
    pthread_mutex_destroy(&dev->async_used_id_mutex);

    // remove the mutex attribute safely
    pthread_mutexattr_destroy(&dev->mutex_attr);
}
