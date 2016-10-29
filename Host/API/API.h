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

#ifndef __API_H__
#define __API_H__

//#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <libusb-1.0/libusb.h>
#include <pthread.h>
#include "../../Protocol/measure.h"
#include "../../Protocol/data_management.h"
#include "CustomUSB.h"

typedef struct {
	// mutexes attributes
	pthread_mutexattr_t mutex_attr;
	
    // mutexes
	pthread_mutex_t handler_mutex;
	pthread_mutex_t measures_queue_mutex;
	pthread_mutex_t exitlock;
	pthread_mutex_t enabled_mutex;
	
	// USB device handler
	libusb_device_handle* handler;
	
	// measures queue
	managed_queue_t measures_queue;
	
	// USB threads
	pthread_t reader;
	pthread_attr_t reader_attr;
	
	bool enabled;
} datachan_device_t;

inline datachan_device_t* datachan_device_setup(libusb_device_handle* native_handle) {
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

inline void datachan_device_cleanup(datachan_device_t* dev) {
	// no need for the thread
	pthread_attr_destroy(&dev->reader_attr);
	
	// remove the mutex safely (acquire and release it first)
	pthread_mutex_destroy(&dev->enabled_mutex);
	pthread_mutex_destroy(&dev->measures_queue_mutex);
	pthread_mutex_destroy(&dev->handler_mutex);
	
	// remove the mutex attribute safely
	pthread_mutexattr_destroy(&dev->mutex_attr);
}

typedef enum {
	uninitialized = 0x00,
	not_found_or_inaccessible,
	cannot_claim,
	malloc_fail,
	unknown,
	success
} search_result_t;

typedef struct {
	search_result_t result;
	datachan_device_t* device;
} datachan_acquire_result_t;

int datachan_is_initialized(void);
void datachan_init(void);
void datachan_shutdown(void);

bool datachan_device_enable(datachan_device_t*);
bool datachan_device_is_enabled(datachan_device_t*);
bool datachan_device_disable(datachan_device_t*);

int datachan_raw_read(datachan_device_t*, uint8_t*);
int datachan_raw_write(datachan_device_t*, uint8_t*, int);

datachan_acquire_result_t device_acquire(void);
void device_release(datachan_device_t**);

void datachan_device_enqueue_measure(datachan_device_t*, const measure_t*);
measure_t* datachan_device_dequeue_measure(datachan_device_t*);
uint32_t datachan_device_enqueue_measures(datachan_device_t*);

#endif // __API_H__
