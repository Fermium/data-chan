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

#include "../Protocol/measure.h"
#include "../config.h"
#include "API.h"

#include <usb.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>

static bool init = false;

int repack_measure(measure_t* out, uint8_t* in) {
   
    //return 0; // error found (using check algorithm)
    return 1;
}

int datachan_init() {
    // no double init
    if (datachan_is_initialized())
        return 1;
    
    // init libusb
    usb_init();
    init = true;

    // success!
    return 1;
}

int datachan_is_initialized() {
    return (init == true) ? 1 : 0;
}
