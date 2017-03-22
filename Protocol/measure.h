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

#ifndef __MEASURE_H__
#define __MEASURE_H__

// Import USB configuration
#include "../config.h"

// Import basic types and functions
#include <stdlib.h>
#include <stdint.h>

#define PROTOCOL_VERSION 0x00

#define REALTIME_MASK 0x80

// commands codes
#define ENABLE_TRANSMISSION     0x01
#define DISABLE_TRANSMISSION    0x02
#define SET_CONFIG_FLAG         0x03

// request/response codes
#define NONE                    0x00 // this is nothing. Nothing to send or receive to/from the device disabled
#define CMD_REQUEST             0x01 // this is a request that must be fulfilled immediatly
#define CMD_ASYNC_RESPONSE      0x02 // this is a response to an async request
#define CMD_ASYNC_REQUEST       0x03 // this is a request that can be fulfilled at any time
#define MEASURE                 0x04 // this is a measure

#define MAX_MEASURE_NUM         8


/********************************************************************************
 *                          measure data structures and macros                  *
 ********************************************************************************/

/*
Measure type:
	0 => non-realtime
	1 => offset-realtime
	2 => realtime

where the MU can be one of the following:
	0 => metre
	1 => ampere
	2 => volt
	3 => coulomb
	4 => watt
	5 => kilogram
	6 => kelvin
	7 => candela
	8 => mole
	9 => hertz
	10 => radian
	11 => steradian
	12 => newton
	13 => pascal
	14 => joule
	15 => farad
	16 => ohm
	17 => siemens
	18 => weber
	19 => tesla
	20 => henry
	21 => lumen
	22 => lux
	23 => becquerel
	24 => gray
	25 => sievert
	26 => katal
and more.... Read the documentation!
*/
typedef struct {
	uint8_t type; // type of measure
	uint8_t mu; // measurement unit (SI) : read above
    uint8_t measuresNum; // number of measures
	uint8_t channels[MAX_MEASURE_NUM]; // the channel of measure, starting from 1, channel zero is reserved
	float values[MAX_MEASURE_NUM]; // float is used because of microcontrollers limitations: https://gcc.gnu.org/wiki/avr-gcc
	uint32_t time; // the UNIX time of the measure
	uint16_t millis; // this is the offset from the given UNIX time expressed in milliseconds
} measure_t;

#define NONREALTIME                     0x00
#define OFFSET_REALTIME                 0x01
#define REALTIME                        0x02
#define PROGRESSIVE                     0x03

#ifdef __HOST__
    void repack_measure(measure_t*, uint8_t*);
#else
    void unpack_measure(measure_t*, uint8_t*);
#endif

// please, stop complaining like a retard shit
measure_t* new_nonrealtime_measure(uint8_t mu);
void add_measure(measure_t* elem, uint8_t ch, float vl);

#endif // __MEASURE_H__
