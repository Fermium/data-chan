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
#include "../Protocol/measure_functions.h"
#include "../Protocol/data_management_functions.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>

#if defined(__MACH__)
	#include <malloc/malloc.h>
#else
    #include <malloc.h>
#endif

libusb_context* ctx = (libusb_context*)NULL;

bool datachan_is_initialized() {
    return (ctx != (libusb_context*)NULL);
}

void datachan_init() {
    // no double init
    if (!datachan_is_initialized()) {
    // init libusb
    if (libusb_init(&ctx) == 0)  {
#ifdef DEBUG
        if (datachan_is_initialized())
            libusb_set_debug(ctx, LIBUSB_LOG_LEVEL_INFO);
#endif // DEBUG
        }
    }
}

void datachan_shutdown(void) {
	// check for uninitialized lib call to avoid segmentation fault
	if (!datachan_is_initialized())
        return;

    // usb shutdown
    libusb_exit(ctx);

    // avoid dangling pointer
    ctx = (libusb_context*)NULL;
}
