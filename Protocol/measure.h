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

#ifndef __MEASURE_H__
#define __MEASURE_H__

// Import USB configuration
#include "../config.h"

// Import basic types and functions
#include <stdlib.h>
#include <stdint.h> 

#define PROTOCOL_VERSION 0x00

#define REALTIME_MASK 0x80

typedef enum {
	GET_PROTOCOL_VERSION 	= 0x00,
    ENABLE_TRANSMISSION     = 0x01,
    DISABLE_TRANSMISSION    = 0x02,
	GET_CONFIG_FLAG 		= 0x03,
	SET_CONFIG_FLAG			= 0x04,
} command_type_t;

typedef enum {
	NONE 					= 0x00,
	CMD_RESPONSE			= 0x01,
	MEASURE					= 0x02
} response_type_t;

/*
Measure type:
	0 => non-realtime
	1 => offset-realtime
	2 => realtime

where the MU can be one of the following:
	0 => meter
	1 => ampere
	2 => volt
	3 => culomb
	4 => watt
*/
typedef struct {
	uint8_t type; // type of measure
	uint8_t mu; // measurement unit (SI) : read above
	uint8_t channel; // the channel of measure, starting from 1, channel zero is reserved
	float value; // float is used because of microcontrollers limitations: https://gcc.gnu.org/wiki/avr-gcc
	uint32_t time; // the UNIX time of the measure
	uint16_t millis; // this is the offset from the given UNXI time expressed as milliseconds
} measure_t;

inline measure_t* new_nonrealtime_measure(uint8_t mu, uint8_t ch, float vl)
{
	// memory allocation
	measure_t* new_elem = (measure_t*)malloc(sizeof(measure_t));
	
	// this is a non-real-time measure
	new_elem->type = mu & (~REALTIME_MASK);
	new_elem->channel = (ch == 0) ? 1 : ch;
	new_elem->value = vl;
	
	// non real-time
	new_elem->time = 0;
	new_elem->millis = 0;
	
	// return the filled struct
    return new_elem;
}

#ifdef __HOST__
	#define REPACK_SUCCESS 				0
	#define REPACK_TRANSMISSION_ERROR 	-1

	int8_t repack_measure(measure_t*, uint8_t*);
#else
    void unpack_measure(measure_t*, uint8_t*);
#endif

struct fifo_queue_t {
	measure_t *measure;
	struct fifo_queue_t *next;
};

inline struct fifo_queue_t* fifo_queue_t(measure_t* m)
{
	struct fifo_queue_t* new_elem = (struct fifo_queue_t*)malloc(sizeof(struct fifo_queue_t));
	new_elem->measure = m;
	new_elem->next = (struct fifo_queue_t*)NULL;
    return new_elem;
}

typedef struct {
	struct fifo_queue_t *first;
	struct fifo_queue_t *last;
} managed_queue_t;

inline void enqueue_measure(managed_queue_t* FIFO, measure_t *measure) {
	// create the new element
	struct fifo_queue_t* newElement = fifo_queue_t(measure);
	
	// the new element MUST be enqueued as the last element
	if (FIFO->first == (struct fifo_queue_t*)NULL) {
		FIFO->first = newElement;
		FIFO->last = newElement;
	} else {
		FIFO->last->next = newElement;
		FIFO->last = FIFO->last->next;
	}
}

inline measure_t *dequeue_measure(managed_queue_t* FIFO) {
	// return NULL if no measures on queue
	if (FIFO->first == (struct fifo_queue_t*)NULL) 
		return (measure_t*)NULL;
	
	// get the first measure added
	struct fifo_queue_t* element = FIFO->first;
	FIFO->first = FIFO->first->next;
		
	// store the measure to be returned
	measure_t* m = element->measure;
		
	// free the memory (RAM is precious as gold is)
	free((void*)element);
		
	// return the measure
	return m;
}

#endif // __MEASURE_H__first
