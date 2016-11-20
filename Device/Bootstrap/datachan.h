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

#ifndef __CUSTOMUSB__
#define __CUSTOMUSB__

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../../Protocol/measure.h"
#include "../../Protocol/data_management.h"

struct request_t {
    uint32_t id;
    void* buffer;
    struct request_t *next;
};

void datachan_init(void);

void datachan_sudden_disconnection(void);

void datachan_generate_report(uint8_t* DataArray);

void datachan_process_report(uint8_t* DataArray);

bool datachan_output_enabled(void);

void datachan_register_measure(measure_t *);

#endif
