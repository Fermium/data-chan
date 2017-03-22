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
#include "BulkVendor.h"
#include "datachan.h"
#include "Settings.h"
#include "Custom.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static bool hostListening = false;

void unpack_measure(measure_t* in, uint8_t* out) {
    // the first byte is the type of measure sent
    memcpy(out, (const void*)&in->type, sizeof(in->type));
    out += sizeof(in->type);

    // channels....
    memcpy((out), (const void*)&in->channels, sizeof(in->channels));
    out += sizeof(in->channels);

    // values...
    memcpy((out), (const void*)&in->values, sizeof(in->values));
    out += sizeof(in->values);

    // number of entries
    memcpy((out), (const void*)&in->measuresNum, sizeof(in->measuresNum));
    out += sizeof(in->measuresNum);

    // ...measurement unit...
    memcpy(out, (const void*)&in->mu, sizeof(in->mu));
    out += sizeof(in->mu);

    if (in->type != NONREALTIME) {
        // ..time...
        memcpy((out), (const void*)&in->time, sizeof(in->time));
        out += sizeof(in->time);

        // ..millis...
        memcpy((out), (const void*)&in->millis, sizeof(in->millis));
        out += sizeof(in->millis);
    }
}

static struct request_t *cmd_queue;

static managed_queue_t FIFO;

void datachan_init(void) {
    cmd_queue = (struct request_t*)NULL;
    FIFO.first = (struct fifo_queue_t *)NULL;
    FIFO.last = (struct fifo_queue_t *)NULL;
    hostListening = false;

    Event_Init();
}

void datachan_sudden_disconnection(void) {
    hostListening = false;
}

/** Function to create the next report to send back to the host at the next reporting interval.
 *
 *  \param[out] DataArray  Pointer to a buffer where the next report data should be stored
 */
void datachan_generate_report(uint8_t* DataArray)
{
    // every unused byte will be NONE
    memset((void*)DataArray, NONE, VENDOR_IO_EPSIZE);

    // by default nothing is sent
    DataArray[0] = (uint8_t)NONE;

    if (hostListening) {

      //if (datachan_output_enabled())datachan_register_measure(new_nonrealtime_measure(0xFF, 1, 169.754699f));
        // get the next measure to be sent over USB
        measure_t* data_to_be_sent = dequeue_measure(&FIFO);

        if (data_to_be_sent != (measure_t*)NULL) {
            // if any flag as present, else flag as 'bad data'
            DataArray[0] = (uint8_t)MEASURE;

            // serialize the measure (for safe transmission)
            unpack_measure(data_to_be_sent, (DataArray + 1));

            // the measure is going to be removed from memory
            free((void*)data_to_be_sent); // save space!
        }// async requests manager
        else if (cmd_queue != (struct request_t*)NULL) {
            // point to the first byte after the response type
            uint8_t *response_builder = DataArray + 1;

            // get the async request to be fulfilled and remove it from the queue
            struct request_t* req = cmd_queue;
            //req->next = (struct request_t*)NULL;
            cmd_queue = cmd_queue->next;

            // flag the response as an async request response
            DataArray[0] = CMD_ASYNC_RESPONSE;

            // write the ID of the request
            memcpy((void*)response_builder, (const void*)&req->id, sizeof(req->id));
            response_builder += sizeof(req->id);

            // generate the buffer response
            uint8_t buffer[GENERIC_REPORT_SIZE - sizeof(req->id) - 1];

            // call the external function that will generate the response
            Process_Async(buffer);

            // apply the generated response
            memcpy((void*)response_builder, (const void*)buffer, sizeof(buffer));

            // remove from the memory the fulfilled async command
            if (req->buffer != NULL) free((void*)req->buffer);
            free((void*)req);
        }
        else {
            DataArray[0] = ((uint8_t)NONE);
        }
    }

    // append the error check byte to the end of EVERY message
    DataArray[GENERIC_REPORT_SIZE] = CRC_calc(DataArray, GENERIC_REPORT_SIZE);
}

/** Function to process the last received report from the host.
 *
 *  \param[in] DataArray  Pointer to a buffer where the last received report has been stored
 */
void datachan_process_report(uint8_t* DataArray)
{
    // on invalid CRC discard the message
    if (!CRC_check(DataArray, GENERIC_REPORT_SIZE, DataArray[GENERIC_REPORT_SIZE]))
            return;

    uint8_t *cmd_builder_buffer;
    uint32_t entry;
    uint16_t data_size = 1;
    uint8_t channel;
    void* data;

    if (DataArray[0] == CMD_REQUEST) {
        uint8_t cmd = DataArray[1];

        // the begin of the cmd buffer is the third byte
        cmd_builder_buffer = DataArray + 2;

        switch (cmd) {
            case ENABLE_TRANSMISSION:
                hostListening = true;
                break;

            case DISABLE_TRANSMISSION:
                hostListening = false;
                break;

            case SET_CONFIG_FLAG:
                // read the setting ID
                memcpy((void*)&entry, (const void*)cmd_builder_buffer, sizeof(entry));
                cmd_builder_buffer += sizeof(entry);

                // read the destination channel
                memcpy((void*)&channel, (const void*)cmd_builder_buffer, sizeof(channel));
                cmd_builder_buffer += sizeof(channel);

                // read the data length
                memcpy((void*)data_size, (const void*)cmd_builder_buffer, sizeof(data_size));
                cmd_builder_buffer += sizeof(data_size);

                // if the length is correct allocate memory
                if (data_size < (VENDOR_IO_EPSIZE - 10)) data = malloc(data_size);

                // read the data
                memcpy((void*)data, (const void*)cmd_builder_buffer, data_size);

                // build the setting data
                memory_block_t mem;
                mem.size = data_size;
                mem.start_addr = malloc(mem.size);
                memcpy(mem.start_addr, (const void*)&data, mem.size);
                setting_entry_t new_set;
                new_set.channel = channel;
                new_set.entry = entry;
                memcpy((void*)(&new_set.setting), (const void*)&mem, sizeof(mem));
                setSetting(&new_set);
                break;

            default:
                break;
        }
    } else if (DataArray[0] == CMD_ASYNC_REQUEST) {
        // the begin of the cmd buffer is the second byte
        cmd_builder_buffer = DataArray + 1;

        // get and store the ID of the request
        memcpy((void*)&entry, (void*)cmd_builder_buffer, sizeof(uint32_t));
        cmd_builder_buffer += sizeof(uint32_t);

        // get and store the request
        data = malloc(VENDOR_IO_EPSIZE - sizeof(uint32_t) - 2);
        memcpy(data, (void*)cmd_builder_buffer, VENDOR_IO_EPSIZE - sizeof(uint32_t) - 2);

        // create and populate the structure that will hold the request
        struct request_t *new_request = (struct request_t*)malloc(sizeof(struct request_t));
        new_request->id = entry;
        new_request->buffer = data;
        new_request->next = (struct request_t*)NULL;

        // append the previous list to the newly created element
        struct request_t* last_populated = cmd_queue;
        new_request->next = last_populated;

        // exchange the old list with the new one
        cmd_queue = new_request;
    } else if (DataArray[0] == NONE) {
        // nothing to do here!
    }
}

bool datachan_output_enabled(void) {
  return hostListening;
}

void datachan_register_measure(measure_t *m) {
  enqueue_measure(&FIFO, m);
}
