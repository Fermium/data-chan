#ifndef __DATA_MANAGEMENT_H__
#define __DATA_MANAGEMENT_H__

#include "measure.h"

struct fifo_queue_t {
	measure_t *measure;
	struct fifo_queue_t *next;
};

typedef struct {
	struct fifo_queue_t *first;
	struct fifo_queue_t *last;
	#ifdef __HOST__
		uint32_t count;
	#endif
} managed_queue_t;


// please, stop complaining like a retard shit
#ifdef __HOST__
    uint32_t count_measures(managed_queue_t* FIFO);
#endif
measure_t *dequeue_measure(managed_queue_t* FIFO);
void enqueue_measure(managed_queue_t* FIFO, measure_t *measure);
struct fifo_queue_t* fifo_queue_t(measure_t* m);
uint8_t CRC_calc(const uint8_t* data, uint16_t len);
uint8_t CRC_check(const uint8_t* data, uint16_t len, uint8_t crc);

#endif // __DATA_MANAGEMENT_H__