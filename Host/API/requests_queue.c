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

void datachan_enqueue_request(datachan_device_t* dev, uint8_t* buf) {
    pthread_mutex_lock(&dev->requests_queue_mutex);
    
    // copy the buffer
    uint8_t* new_req = (uint8_t*)malloc(GENERIC_REPORT_SIZE - 1);
    memcpy((void*)new_req, buf, GENERIC_REPORT_SIZE - 1);
    
    // create and populate the list element
    struct bulk_out_t* new_out = (struct bulk_out_t*)malloc(sizeof(struct bulk_out_t));
    new_out->buffer = new_req;
    new_out->next = (struct bulk_out_t*)NULL;
    
    if (dev->requests_queue == (struct bulk_out_t*)NULL) {
        dev->requests_queue = new_out;
    } else {
        // go to the end of the list
        struct bulk_out_t* current_out = dev->requests_queue;
        while (current_out->next != (struct bulk_out_t*)NULL)
            current_out = current_out->next;
        
        // and append the new element
        current_out->next = new_out;
    }
    
    pthread_mutex_unlock(&dev->requests_queue_mutex);
}

void datachan_dequeue_request(datachan_device_t* dev, uint8_t* buf) {
    // the default empty packet
    uint8_t out_packet[GENERIC_REPORT_SIZE - 1];
    memset(out_packet, NONE, sizeof(out_packet));
    
    
    // try to replace the default packet
    pthread_mutex_lock(&dev->requests_queue_mutex);
    
    if (dev->requests_queue != (struct bulk_out_t*)NULL) {
        // get the current bulk buffer to be returned
        struct bulk_out_t* current_out = dev->requests_queue;
        
        // remove from the list the removed bulk buffer
        dev->requests_queue = dev->requests_queue->next;
        
        // copy the buffer to be returned
        memcpy(out_packet, current_out->buffer, GENERIC_REPORT_SIZE - 1);
        
        // remove memory
        free((void*)current_out->buffer);
        free((void*)current_out);
    }
    
    pthread_mutex_unlock(&dev->requests_queue_mutex);
    
    
    // return the obtained data
    memcpy(buf, out_packet, sizeof(out_packet));
}