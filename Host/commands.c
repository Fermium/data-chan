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

void datachan_send_sync_command(datachan_device_t* dev, uint8_t cmdType, uint8_t *cmdBuf, uint8_t cmdBufLength) {
    // allocate space for the sync command buffer
    uint8_t cmd[GENERIC_REPORT_SIZE] = { CMD_REQUEST };
    
    // start building the command buffer
    uint8_t *cmd_builder_buffer = cmd + 1;
    
    // write the command type
    *(cmd_builder_buffer) = cmdType;
    cmd_builder_buffer++;
    
    // fill unused space with zeroes (minus two because cmd_builder_buffer points to cmd[2])
    memset(cmd_builder_buffer, 0, sizeof(cmd) - 2);
    
    // avoid buffer overflow
    cmdBufLength = (cmdBufLength > GENERIC_REPORT_SIZE - 2) ? GENERIC_REPORT_SIZE - 2 : cmdBufLength;
    
    // copy the command buffer if the memory is valid
    if (cmdBuf != (uint8_t*)NULL)
        memcpy((void*)cmd_builder_buffer, (const void*)cmdBuf, cmdBufLength);
    
    // enqueue the config cmd
    datachan_enqueue_request(dev, cmd);
}

void datachan_send_async_command(datachan_device_t* dev, uint8_t cmdType, uint8_t *cmdBuf, uint8_t cmdBufLength) {
    // allocate space for the sync command buffer
    uint8_t cmd[GENERIC_REPORT_SIZE] = { CMD_ASYNC_REQUEST };
    
    // start building the command buffer
    uint8_t *cmd_builder_buffer = cmd + 1;
    
    // increase the counter of used async ID's
    pthread_mutex_lock(&dev->async_used_id_mutex);
    uint32_t current_id = dev->async_used_id++;
    pthread_mutex_unlock(&dev->async_used_id_mutex);

    // place the ID on the current request
    memcpy(cmd_builder_buffer, (const void*)&current_id, sizeof(dev->async_used_id));
    cmd_builder_buffer += sizeof(dev->async_used_id);

    // write the command type
    *(cmd_builder_buffer) = cmdType;
    cmd_builder_buffer++;
    
    // fill unused space with zeroes (minus two because cmd_builder_buffer points to cmd[2])
    memset(cmd_builder_buffer, 0, sizeof(cmd) - 2 - sizeof(dev->async_used_id));
    
    // avoid buffer overflow
    cmdBufLength = (cmdBufLength > (GENERIC_REPORT_SIZE - 2 - sizeof(dev->async_used_id))) ? (GENERIC_REPORT_SIZE - 2 - sizeof(dev->async_used_id)): cmdBufLength;
    
    // copy the command buffer if the memory is valid
    if (cmdBuf != (uint8_t*)NULL)
        memcpy((void*)cmd_builder_buffer, (const void*)cmdBuf, cmdBufLength);
    
    // enqueue the config cmd
    datachan_enqueue_request(dev, cmd);
}
