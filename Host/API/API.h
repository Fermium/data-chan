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

#ifndef __API_H__
#define __API_H__

#include <inttypes.h>

typedef struct {
    void* handler;
} datachan_device_t;

typedef enum {
	uninitialized = 0x00,
	not_found_or_inaccessible,
	cannot_config,
	cannot_claim,
	malloc_fail,
	unknown,
	success
} search_result_t;

typedef struct {
	search_result_t result;
	datachan_device_t* device;
} datachan_acquire_result_t;

int datachan_is_initialized(void);
void datachan_init(void);
void datachan_shutdown(void);

datachan_acquire_result_t acquire_device(void);
void release_device(datachan_device_t**);

#endif // __API_H__
