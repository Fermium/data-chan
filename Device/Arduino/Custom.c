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

#include <Arduino.h> //yeah!

#include <Custom.h>

/*extern "C"*/ void Process_Async(uint8_t* data) {
  /*
    data is the content of the packet minus the CRC, the CMD_ASYNC_RESPONSE,
    and the ID.... so data have (GENERIC_REPORT_SIZE - 1 - 4) size
  */
}

/*extern "C"*/ void Event_Connected(void) {

}

/*extern "C"*/ void Event_Disconnected(void) {

}

/*extern "C"*/ void Event_Init(void) {

}

/*extern "C"*/ void MainRoutine(void) {
  // An example of measure generation :)
  if (datachan_output_enabled())
    datachan_register_measure(new_nonrealtime_measure(0xFF, 1, 169.754699f));
}

