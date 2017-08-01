/*
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

#ifdef _MSC_VER
  #define _CRT_SECURE_NO_WARNINGS 1
#endif

#if _MSC_VER
	//disable warnings on dll interface unexported
	#pragma warning (disable : 4275)
	//disable warnings on dll interface unexported
	#pragma warning (disable : 4251)

	//disable warnings on incomplete type sqlite3
	#pragma warning (disable : 4150)
	//disable warnings on 255 char debug symbols
	#pragma warning (disable : 4786)
	//disable warnings on extern before template instantiation
	#pragma warning (disable : 4231)
#endif


#ifdef _WIN32
	#ifdef DATACHAN_EXPORT
		#define DATACHAN_API __declspec(dllexport)
	#else
		#define DATACHAN_API __declspec(dllimport)
	#endif

#else
        #define DATACHAN_API
#endif

#if __GNUC__ >= 4
	#ifdef DATACHAN_EXPORT
		#define DLL_LOCAL __attribute__ ((visibility ("hidden")))
	#else
		#define DLL_LOCAL
	#endif
#else
	#define DLL_LOCAL
#endif

#include <stdint.h>
#include <stdbool.h>
#include <libusb-1.0/libusb.h>
#include <pthread.h>
#include "Protocol/measure.h"
#include "Protocol/data_management.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__HOST__)
struct bulk_out_t {
    uint8_t *buffer;
    struct bulk_out_t *next;
};
#endif

typedef struct {
    // mutexes attributes
    pthread_mutexattr_t mutex_attr;

    // mutexes
    pthread_mutex_t handler_mutex;
    pthread_mutex_t measures_queue_mutex;
    pthread_mutex_t exitlock;
    pthread_mutex_t enabled_mutex;
    pthread_mutex_t requests_queue_mutex;
    pthread_mutex_t async_used_id_mutex;
    
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
    void *requests_queue;

    // A counter for the used ID
    uint32_t async_used_id;
} datachan_device_t;

/*
 *      Struct helpers
 */

DLL_LOCAL datachan_device_t* datachan_device_setup(libusb_device_handle* native_handle);
DLL_LOCAL void datachan_device_cleanup(datachan_device_t* dev);

typedef enum {
    uninitialized 				= 0x00,
    not_found_or_inaccessible	= 0x01,
    cannot_claim				= 0x02,
    malloc_fail					= 0x03,
    unknown						= 0x04,
    success						= 0xFF,
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

/**
 * @brief Check library status
 * 
 * Check if the library has been initialized and was not
 * shutdown before this function call.
 * 
 * @return TRUE if the library is in a working state
 */
DATACHAN_API bool datachan_is_initialized(void);

/**
 * @brief Initialize the library
 *
 * Initialize the library and its underlying components.
 * This function __must__ be called before any other function,
 * except datachan_is_initialized().
 *
 * @see datachan_is_initialized
 */
DATACHAN_API void datachan_init(void);

/**
 * @brief Shutdown the library
 *
 * Shutdown the library and its underlying components.
 * This function __should__ be called just before the main
 * program exit.
 */
DATACHAN_API void datachan_shutdown(void);


/*
 *      Acquire the device: device_management.c
 */

/**
 * @brief Logically acquire a device
 *
 * Acquire a device with the specified VID and PID:
 * it is necessary that the OS provides permission,
 * in order to access and use an USB device.
 *
 * @param vid the Vendor ID of the USB device
 * @param pid the ProductID of the USB device
 * @return the result of the operation and, on succes, a pointer to the device structure
 */
DATACHAN_API datachan_acquire_result_t datachan_device_acquire(uint16_t vid, uint16_t pid);

/**
 * @brief Logically free an acquired device
 *
 * A device that has been acquired __must__ be released,
 * freeing all its used resources, before exiting the program.
 * 
 * @param dev the handle to the device to be released
 */
DATACHAN_API void datachan_device_release(datachan_device_t* dev);


/*
 *      Enable/Disable the device output (IN packets): device_enabler.c
 */

/**
 * @brief Enable data transmission from the device
 * 
 * Until this function is called the device will never send a
 * meaningful IN packet.
 *
 * @param dev the handle of the device to enable
 * @return TRUE on success, FALSE if something went wrong or the handle is not valid
 */
DATACHAN_API bool datachan_device_enable(datachan_device_t* dev);

/**
 * @brief Check if the device transmission is enabled
 * 
 * Check if the data transmission from the given
 * device has been previously enabled and has not
 * been disabled.
 * 
 * @param dev the handle of the device to check
 * @return TRUE if the device is currenty enabled, FALSE otherwise
 */
DATACHAN_API bool datachan_device_is_enabled(datachan_device_t* dev);

/**
 * @brief Disable data transmission from the device
 * 
 * Restore the device in its default state.
 * After this function is called the device will never send a
 * meaningful IN packet.
 *
 * @param dev the handle of the device to disable
 * @return TRUE on success, FALSE if something went wrong or the handle is not valid
 */
DATACHAN_API bool datachan_device_disable(datachan_device_t* dev);


/*
 *      Enqueue generic buffer (OUT packet content): requests_queue.c
 */

DLL_LOCAL void datachan_enqueue_request(datachan_device_t*, uint8_t*);
DLL_LOCAL void datachan_dequeue_request(datachan_device_t*, uint8_t*);


/*
 *      Simple genaration of OUT packets: commands.c
 */

/**
 * @brief Send a synchronous request to the device
 * 
 * Send a synchronous request to the device at the right time.
 * This function generate the correct structure for the specified
 * command and place the command in a FIFO queue.
 * 
 * @param dev the handle of the device
 * @param cmdType the byte that identify the command
 * @param cmdBuf the expansion buffer for the command
 * @param cmdBufLength the length of the expansion buffer
 */
DATACHAN_API void datachan_send_sync_command(datachan_device_t* dev, uint8_t cmdType, uint8_t* cmdBuf, uint8_t cmdBufLength);

DATACHAN_API void datachan_send_async_command(datachan_device_t* dev, uint8_t cmdType, uint8_t* cmdBuf, uint8_t cmdBufLength);

/*
 *      Measures functions: measures.c
 */

DLL_LOCAL void datachan_device_enqueue_measure(datachan_device_t*, const measure_t*);
DATACHAN_API measure_t* datachan_device_dequeue_measure(datachan_device_t* dev);
DATACHAN_API int32_t datachan_device_enqueued_measures(datachan_device_t*);

/**
 * Remove a measure from memory.
 *
 * Avoid memory leaks calling this function when a measure isn't needed
 * anymore.
 *
 * @param measure the measure to be removed from memory
 */
DATACHAN_API void datachan_clean_measure(measure_t* measure);

DATACHAN_API void datachan_device_set_config(datachan_device_t*, uint32_t, uint8_t, void*, uint16_t);

#ifdef __cplusplus
}
#endif

#endif // __API_H__
