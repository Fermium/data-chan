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

#include "Custom.h"

#ifndef EXTLIB

void Process_Async(uint8_t* inData, uint8_t* outData) {
  /*
    inData is the content of the packet minus the CRC and the command ID.

    outData will be the output buffer, without the CMD_ASYNC_RESPONSE,
    the ID and the CRC........ so outData is (GENERIC_REPORT_SIZE - 1 - 1 - 4) in size:

    -1 reserved to the CMD_ASYNC_RESPONSE
    -1 reserved to the CRC
    -1 reserved to the ID
  */
}

void Event_Connected(void) {

}

void Event_Disconnected(void) {

}

void Event_Init(void) {

}

void MainRoutine(void) {
  // An example of measure generation :)
  if (datachan_output_enabled()) {
    measure_t* test_measure = new_nonrealtime_measure(0xFF);

    add_measure(test_measure, 0, 173.345);
    add_measure(test_measure, 1, 45.5);
    add_measure(test_measure, 2, 56.12);

    datachan_register_measure(test_measure);

  }
}

#endif // EXTLIB
