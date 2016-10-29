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

#include "../../Protocol/measure_functions.h"
#include "../../Protocol/data_management_functions.h"
#include "CustomUSB.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static bool hostListening = false;

void unpack_measure(measure_t* in, uint8_t* out) {
    // the first byte is the type of measure sent
    memcpy(out, (const void*)&in->type, sizeof(in->type));
    out += sizeof(in->type);

    // the second byte is the source channel
    memcpy((out), (const void*)&in->channel, sizeof(in->channel));
    out += sizeof(in->channel);

    // let's continue with the value...
    memcpy((out), (const void*)&in->value, sizeof(in->value));
    out += sizeof(in->value);

    // ...measurement unit...
    memcpy(out, (const void*)&in->mu, sizeof(in->mu));
    out += sizeof(in->mu);

    // ..time...
    memcpy((out), (const void*)&in->time, sizeof(in->time));
    out += sizeof(in->time);

    // ..millis...
    memcpy((out), (const void*)&in->millis, sizeof(in->millis));
    out += sizeof(in->millis);
}

static managed_queue_t FIFO;

void datachan_init(void) {
    FIFO.first = (struct fifo_queue_t *)NULL;
    FIFO.last = (struct fifo_queue_t *)NULL;
}

/** Function to create the next report to send back to the host at the next reporting interval.
 *
 *  \param[out] DataArray  Pointer to a buffer where the next report data should be stored
 */
void CreateGenericHIDReport(uint8_t* DataArray)
{
	/*
		This is where you need to create reports to be sent to the host from the device. This
		function is called each time the host is ready to accept a new report. DataArray is
		an array to hold the report to the host.
	*/

	// every unused byte will be zero
	memset((void*)DataArray, 0, GENERIC_REPORT_SIZE);

	// by default nothing is sent
	DataArray[0] = (uint8_t)NONE;	

	if (hostListening) {
		// testing purpouse ONLY!
		enqueue_measure(&FIFO, new_nonrealtime_measure(0xFF, 1, 169.754699f));
	
		// get the next measure to be sent over USB
		measure_t* data_to_be_sent = dequeue_measure(&FIFO);
	
		// if any flag as present, else flag as 'bad data'
		DataArray[0] = (data_to_be_sent == (measure_t*)NULL) ? ((uint8_t)NONE) : ((uint8_t)MEASURE);
	
		// serialize the measure (for safe transmission)
		unpack_measure(data_to_be_sent, (DataArray + 1));
	
		// the measure is going to be removed from memory
		free((void*)data_to_be_sent); // save space!
	}
        
        
        
    // append the error check byte to the end of EVERY message
    DataArray[GENERIC_REPORT_SIZE - 1] = CRC_calc(DataArray, GENERIC_REPORT_SIZE - 1);
}

/** Function to process the last received report from the host.
 *
 *  \param[in] DataArray  Pointer to a buffer where the last received report has been stored
 */
void ProcessGenericHIDReport(uint8_t* DataArray)
{
	/*
		This is where you need to process reports sent from the host to the device. This
		function is called each time the host has sent a new report. DataArray is an array
		holding the report sent from the host.
	*/

	if (DataArray[0] == CMD_MAGIC_FLAG) {
		uint8_t cmd = DataArray[1];
		
		switch (cmd) {
			case ENABLE_TRANSMISSION:
				hostListening = true;
				break;
			case DISABLE_TRANSMISSION:
				hostListening = false;
				break;
			
			default:
				break;
		}
	}
}

