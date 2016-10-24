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

#ifndef __DATACHAN_CONFIG_H__
#define __DATACHAN_CONFIG_H__

#ifndef __HOST__
	#define __DEVICE__
#endif

// USB Device Configuration
#define USB_VID 				0x03EB
#define USB_PID 				0x204F
#define USB_MANUFACTURE			L"Dio"
#define USB_NAME				L"CazzoDuro 8=====D"
#define GENERIC_REPORT_SIZE 	0x6A

#endif // __DATACHAN_CONFIG_H__
