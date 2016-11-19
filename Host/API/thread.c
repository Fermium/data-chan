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

#include "../../config.h"
#include "API.h"

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

void* IO_bulk_thread(void* device) {
    if (device == NULL)
        pthread_exit(NULL);

    datachan_device_t* dev = (datachan_device_t*)device;
    uint8_t data_in[GENERIC_REPORT_SIZE];
    int data_size;
    measure_t m;

    while (datachan_device_is_enabled(dev)) {
        data_size = datachan_raw_read((datachan_device_t*)dev, data_in);

        // deserialize the received measure only if it really is a valid measure
        if ((data_size > 0) && (*(data_in) == MEASURE)) {
            // deserialize the measure and insert the result into the queue
            repack_measure(&m, data_in + 1);
            datachan_device_enqueue_measure(dev, (const measure_t*)&m);
        }
    }

    return NULL;
}
