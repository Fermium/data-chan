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

#include <stdint.h>
#include <stdbool.h>
#include <libusb-1.0/libusb.h>
#include <pthread.h>
#include "../../Protocol/measure.h"
#include "../../Protocol/data_management.h"

#ifdef __cplusplus
extern "C" {
#endif

struct bulk_out_t {
    uint8_t *buffer;
    struct bulk_out_t *next;
};

typedef struct {
    // mutexes attributes
    pthread_mutexattr_t mutex_attr;

    // mutexes
    pthread_mutex_t handler_mutex;
    pthread_mutex_t measures_queue_mutex;
    pthread_mutex_t exitlock;
    pthread_mutex_t enabled_mutex;
    pthread_mutex_t requests_queue_mutex;
    
    // USB device handler
    libusb_device_handle* handler;

    // measures queue
    managed_queue_t measures_queue;

    // USB threads
    pthread_t reader;
    pthread_attr_t reader_attr;

    // Is the device enabled?
    bool enabled;
    
    // The requests queue
    struct bulk_out_t *requests_queue;
} datachan_device_t;

/*
 *      Struct helpers
 */

datachan_device_t* datachan_device_setup(libusb_device_handle* native_handle);
void datachan_device_cleanup(datachan_device_t* dev);

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

#define USB_USED_INTERFACE      0
#define USB_IN_ENDPOINT         0x83
#define USB_OUT_ENDPOINT        0x04
#define TIMEOUT_MS              1000

/*
 *      Initialize and shutdown the library: lib_init.c
 */

bool datachan_is_initialized(void);
void datachan_init(void);
void datachan_shutdown(void);

/*
 *      Acquire the device (detaching stub OS drivers): device_management.c
 */

datachan_acquire_result_t datachan_device_acquire(void);
void datachan_device_release(datachan_device_t*);

/*
 *      Enable/Disable the device output (IN packets): device_enabler.c
 */

bool datachan_device_enable(datachan_device_t*);
bool datachan_device_is_enabled(datachan_device_t*);
bool datachan_device_disable(datachan_device_t*);

/*
 *      Enqueue generic buffer (OUT packet content): requests_queue.c
 */

void datachan_enqueue_request(datachan_device_t*, uint8_t*);
void datachan_dequeue_request(datachan_device_t*, uint8_t*);

/*
 *      Simple genaration of OUT packets: commands.c
 */
void datachan_send_sync_command(datachan_device_t*, uint8_t, uint8_t*, uint8_t);

/*
 *      Measures functions: measures.c
 */

void datachan_device_enqueue_measure(datachan_device_t*, const measure_t*);
measure_t* datachan_device_dequeue_measure(datachan_device_t*);
int32_t datachan_device_enqueued_measures(datachan_device_t*);

void datachan_device_set_config(datachan_device_t*, uint32_t, uint8_t, void*, uint16_t);

#ifdef __cplusplus
}
#endif

#endif // __API_H__
