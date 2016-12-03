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

#include "Device.hpp"
#include <string.h>
#include <stdlib.h>

Device::Device(void) {
    // attempt to open the device
    datachan_acquire_result_t scan_result = datachan_device_acquire();
    
    if (scan_result.result == success) {
        // get the device handler
        this->dev = scan_result.device;
    } else if (scan_result.result == uninitialized) {
        throw new DeviceAcquisitionException(ACQUISITION_ERR_UNINITIALIZED_LIB);
    } else if (scan_result.result == not_found_or_inaccessible) {
        throw new DeviceAcquisitionException(ACQUISITION_ERR_NO_DEVICE);
    } else if (scan_result.result == cannot_claim) {
        throw new DeviceAcquisitionException(ACQUISITION_ERR_CANNOT_CLAIM);
    } else if (scan_result.result == malloc_fail) {
        throw new DeviceAcquisitionException(ACQUISITION_ERR_BAD_MALLOC);
    } else {
        throw new DeviceAcquisitionException(ACQUISITION_ERR_UNKNOWN);
    }
}

Device::~Device(void) {
    if (this->dev != (datachan_device_t*)NULL)
        datachan_device_release(&this->dev);
}

void Device::Enable(void) {
    //check for device
    if (this->dev == (datachan_device_t*)NULL)
        throw new NoDeviceException();
    
    datachan_device_enable(this->dev);
}

void Device::Disable(void) {
    //check for device
    if (this->dev == (datachan_device_t*)NULL)
        throw new NoDeviceException();
    
    datachan_device_disable(this->dev);
}

bool Device::IsEnabled(void) {
    //check for device
    if (this->dev == (datachan_device_t*)NULL)
        throw new NoDeviceException();
    
    return datachan_device_is_enabled(this->dev);
}

uint32_t Device::CountEnqueuedMeasures(void) {
    //check for device
    if (this->dev == (datachan_device_t*)NULL)
        throw new NoDeviceException();
    
    return datachan_device_enqueued_measures(this->dev);
}

Measure* Device::GetEnqueuedMeasure(void) {
    //check for device
    if (this->dev == (datachan_device_t*)NULL)
        throw new NoDeviceException();
    
    // this variable is loaded onto the stack
    measure_t staticMeasure;
    
    // read the measure (value is on the heap)
    measure_t* measure = datachan_device_dequeue_measure(this->dev);
    
    // copy the measure from the heap to the stack
    memcpy((void*)&staticMeasure, (const void*)measure, sizeof(measure_t));

    // deallocate memory from the heap (avoid memory leaks)
    free((void*)measure);
    
    // create and return the managed measure structure
    return new Measure(&staticMeasure);
}
