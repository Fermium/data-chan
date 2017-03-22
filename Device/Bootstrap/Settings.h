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

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
	uint32_t size;
	void* start_addr;
} memory_block_t;

typedef struct {
	uint32_t entry;
	uint8_t channel;
	memory_block_t setting;
} setting_entry_t;

struct setting_queue_t {
	setting_entry_t *entry;
	struct setting_queue_t *next;
};

#ifdef __cplusplus
extern "C" {
#endif

void setSetting(const setting_entry_t*);
memory_block_t *getSetting(uint32_t, uint8_t);

#ifdef __cplusplus
}
#endif

#endif
