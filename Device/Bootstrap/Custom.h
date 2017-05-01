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

#ifndef __CUSTOM__
#define __CUSTOM__

#ifndef __ARDUINO__
    #include "../../Protocol/measure.h"
    #include "../../Protocol/data_management.h"
    #include "datachan.h"
    #include "Settings.h"
    #include <stdint.h>
    #include <string.h>
    #include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

void Process_Async(uint8_t*, uint8_t*);
void Event_Init(void);
void Event_Connected(void);
void Event_Disconnected(void);
void MainRoutine(void);

#ifdef __cplusplus
}
#endif

#endif // __CUSTOM__
