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


using namespace DataChan;

Device::Device() {
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

Device::~Device() {
    datachan_device_release(&this->dev);
}