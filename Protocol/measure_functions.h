#ifndef __MEASURE_FUNCTIONS__
#define __MEASURE_FUNCTIONS__

#include "measure.h"

inline measure_t* new_nonrealtime_measure(uint8_t mu)
{
	// memory allocation
	measure_t* new_elem = (measure_t*)malloc(sizeof(measure_t));
	
	// this is a non-real-time measure
	new_elem->type = NONREALTIME;
	new_elem->mu = mu;

    // no measures
    new_elem->measuresNum = 0;    

	/*new_elem->channel = (ch == 0) ? 1 : ch;
	new_elem->value = vl;*/
	
	// non real-time
	new_elem->time = 0;
	new_elem->millis = 0;
	
	// return the filled struct
    return new_elem;
}

inline void add_measure(measure_t* elem, uint8_t ch, float vl)
{
    elem->channels[elem->measuresNum] = vl;
    elem->values[elem->measuresNum] = vl;

    elem->measuresNum++;
}

#endif
