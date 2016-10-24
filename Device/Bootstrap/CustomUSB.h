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

#ifndef __CUSTOMUSB__
#define __CUSTOMUSB__

#include <stdlib.h>
#include <stdint.h> 
#include "../../Protocol/measure.h"

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

void enqueue_measure(measure_t *);

measure_t *dequeue_measure(void);

void CreateGenericHIDReport(uint8_t* DataArray);

void ProcessGenericHIDReport(uint8_t* DataArray);

#endif