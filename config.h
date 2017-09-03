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

#ifndef __DATACHAN_CONFIG_H__
#define __DATACHAN_CONFIG_H__

#ifndef __HOST__
	#define __DEVICE__
#endif

#ifdef __DEVICE__
	// USB Device Configuration
	#ifndef USB_VID
		#define USB_VID 0x03EB
	#endif
	#ifndef USB_PID
		#define USB_PID 0x204F
	#endif
	#ifndef USB_MANUFACTURER
		#define USB_MANUFACTURER L"MyManufacturer"
	#endif
	#ifndef USB_NAME
		#define USB_NAME L"datachan tester"
	#endif
#endif

// following macros are standard for data-chan. Any other nuber will not work!
#define GENERIC_REPORT_SIZE                     63 // do not change: the official library will refuse to work!
#define GENERIC_POLL_INTERVAL_MS                0x01 // originally was 0x05

/*
65 can also be used (but using a 65b packet means 35 measures each second,
whereas using a 25b packet means that the software is able to transfert
more than 83 measures each second).

Measures done on a Linux kernel 4.4 with an atmega32u4
*/

#endif // __DATACHAN_CONFIG_H__
