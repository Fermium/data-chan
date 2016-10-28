#ifndef __DATA_MANAGEMENT_H__
#define __DATA_MANAGEMENT_H__

#include "measure.h"

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
	#ifdef __HOST__
		uint32_t count;
	#endif
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
	
	#ifdef __HOST__
		FIFO->count++;
	#endif
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
	
	#ifdef __HOST__
		FIFO->count--;
	#endif
	
	// return the measure
	return m;
}

#ifdef __HOST__
inline uint32_t count_measures(managed_queue_t* FIFO) {
	// return the measure
	return FIFO->count--;
}
#endif

#endif // __DATA_MANAGEMENT_H__