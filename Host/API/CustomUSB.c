#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "CustomUSB.h"

int8_t repack_measure(measure_t* out, uint8_t* in) {
    //save the starting address
	uint8_t *starting_addr = in;

    // the first byte is the type of measure sent
	memcpy((void*)&out->type, (const void*)in, sizeof(out->type));
	in += sizeof(out->type);

	// the second byte is the source channel
	memcpy((void*)&out->channel, (const void*)in, sizeof(out->channel));
	in += sizeof(out->channel);
	
	// let's continue with the value...
	memcpy((void*)&out->value, (const void*)in, sizeof(out->value));
	in += sizeof(out->value);

	// ...measurement unit...
	memcpy((void*)&out->mu, (const void*)in, sizeof(out->mu));
	in += sizeof(out->mu);

	// ..time...
	memcpy((void*)&out->time, (const void*)in, sizeof(out->time));
	in += sizeof(out->time);

	// ..millis...
	memcpy((void*)&out->millis, (const void*)in, sizeof(out->millis));
	in += sizeof(out->millis);

	// append the error check byte
	if (CRC_check(
				starting_addr,
				sizeof(measure_t), 
				CRC_calc(starting_addr, sizeof(measure_t)))
			) return REPACK_SUCCESS;

	return REPACK_TRANSMISSION_ERROR;
}
