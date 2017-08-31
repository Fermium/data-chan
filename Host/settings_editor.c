/**
	
	Copyright (C) 2017 Fermium LABS srl

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

#include "../config.h"
#include "DataChan.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>

void datachan_device_set_config(datachan_device_t* dev, uint32_t entry, uint8_t channel, void* data, uint16_t data_size) {
    //this is the data buffer
    uint8_t cmd[GENERIC_REPORT_SIZE] = { CMD_REQUEST, SET_CONFIG_FLAG };
    uint8_t *cmd_builder_buffer = cmd + 2;

    // fill unused space with zeroes (minus two because cmd_builder_buffer points to cmd[2])
    memset(cmd_builder_buffer, 0, sizeof(cmd) - 2);

    // write the setting ID
    memcpy((void*)cmd_builder_buffer, (const void*)&entry, sizeof(entry));
    cmd_builder_buffer += sizeof(entry);

    // write the destination channel
    memcpy((void*)cmd_builder_buffer, (const void*)&channel, sizeof(channel));
    cmd_builder_buffer += sizeof(channel);

    // prevent buffer overflow (10 means: CMD_REQUEST(1), SET_CONFIG_FLAG(1), entry(4), channel(1), data_size(2), CRC(1) )
    data_size = (data_size > (GENERIC_REPORT_SIZE - 10)) ? GENERIC_REPORT_SIZE - 10 : data_size;

    // write the data length
    memcpy((void*)cmd_builder_buffer, (const void*)&data_size, sizeof(data_size));
    cmd_builder_buffer += sizeof(data_size);

    // write the data
    memcpy((void*)cmd_builder_buffer, (const void*)data, data_size);

    // enqueue the config cmd
    datachan_enqueue_request(dev, cmd);
}
