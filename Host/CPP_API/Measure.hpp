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

#ifndef MEASURE_HPP
#define MEASURE_HPP

#ifdef __cplusplus
extern "C" {
#endif

#include "../API/API.h"
#include "InvalidMeasureException.hpp"
#include <cstddef>
#include <stdint.h>
#include <stdbool.h>
#include <libusb-1.0/libusb.h>
#include <pthread.h>

namespace DataChan {
    class Measure {
    public:
        Measure(measure_t *nativeMeasure);
        Measure(const Measure& orig);
        virtual ~Measure();

        enum Type {
            NonRealtime     = NONREALTIME,
            OffsetRealtime  = OFFSET_REALTIME,
            Progressive     = PROGRESSIVE,
            Realtime        = REALTIME
        };
        
        enum MeasurementUnit {
            Meter = 0x00,
            Ampere = 0x01,
            Volt = 0x02,
            Coulomb = 3,
            Watt = 4,
            Kilogram = 5,
            Kelvin = 6,
            Candela = 7,
            Mole = 8,
            Hertz = 9,
            Radian = 10,
            Steradian = 11,
            Newton = 12,
            Pascal = 13,
            Joule = 14,
            Farad = 15,
            Ohm = 16,
            Siemens = 17,
            Weber = 18,
            Tesla = 19,
            Henry = 20,
            Lumen = 21,
            Lux = 22,
            Becquerel = 23,
            Gray = 24,
            Sievert = 25,
            Katal = 26,
        };

    private:
        Measure::Type type;
        Measure::MeasurementUnit measurementUnit;
    };
}

#ifdef __cplusplus
}
#endif

#endif /* MEASURE_HPP */

