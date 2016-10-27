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
#include "../../Protocol/measure.h"
#include "../../Protocol/data_management.h"
#include "CustomUSB.h"

typedef struct {
    void* handler;
	managed_queue_t unread_measures_queue;
	bool enabled;
} datachan_device_t;

inline datachan_device_t* new_datachan_device_t(void* native_handle) {
	// allocate memory for the device
	datachan_device_t* dev = (datachan_device_t*)malloc(sizeof(datachan_device_t));
	
	// register the native handle
	dev->handler = native_handle;

	// prepare the internal queue
	dev->unread_measures_queue.first = (struct fifo_queue_t *)NULL;
	dev->unread_measures_queue.last = (struct fifo_queue_t *)NULL;
	
	// the device is disabled
	dev->enabled = false;
	
	// enjoy the device
	return dev;
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

int datachan_device_enable(datachan_device_t*);
int datachan_device_disable(datachan_device_t*);

int datachan_raw_read(datachan_device_t*, uint8_t*);
int datachan_raw_write(datachan_device_t*, uint8_t*, int);

datachan_acquire_result_t device_acquire(void);
void device_release(datachan_device_t**);

#endif // __API_H__
