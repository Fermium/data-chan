#ifndef __MEASURE_FUNCTIONS__
#define __MEASURE_FUNCTIONS__

#include "measure.h"

inline measure_t* new_nonrealtime_measure(uint8_t mu, uint8_t ch, float vl)
{
	// memory allocation
	measure_t* new_elem = (measure_t*)malloc(sizeof(measure_t));
	
	// this is a non-real-time measure
	new_elem->type = NONREALTIME;
	new_elem->mu = mu;
	new_elem->channel = (ch == 0) ? 1 : ch;
	new_elem->value = vl;
	
	// non real-time
	new_elem->time = 0;
	new_elem->millis = 0;
	
	// return the filled struct
    return new_elem;
}

inline uint8_t CRC_calc(const uint8_t* data, uint16_t len) {
	// generate the check (to identify transmission errors)
	uint8_t crc = 0x00;
	while (len--) {
		uint8_t extract = *data++;
		for (uint8_t tempI = 8; tempI; tempI--) {
			uint8_t sum = (crc ^ extract) & 0x01;
			crc >>= 1;
			if (sum) crc ^= 0x8C;
			extract >>= 1;
		}
	}
	return crc;
}

inline uint8_t CRC_check(const uint8_t* data, uint16_t len, uint8_t crc) {
	return (crc == CRC_calc(data, len));
}

//#ifdef __HOST__
    // please, stop complaining like a retard shit
    measure_t* new_nonrealtime_measure(uint8_t mu, uint8_t ch, float vl);
    uint8_t CRC_calc(const uint8_t* data, uint16_t len);
    uint8_t CRC_check(const uint8_t* data, uint16_t len, uint8_t crc);
//#endif

#endif