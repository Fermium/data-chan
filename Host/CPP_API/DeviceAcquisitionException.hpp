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

#ifndef DEVICEACQUISITIONEXCEPTION_HPP
#define DEVICEACQUISITIONEXCEPTION_HPP

#include <cstddef>
#include <exception>
#include "DataChan.hpp"

DATACHAN_CPP_API class DeviceAcquisitionException : public std::exception {
    public:
        DeviceAcquisitionException(const char* err_str);
        const char* what();
        
    private:
        const char* errStr;
};

const char ACQUISITION_ERR_UNINITIALIZED_LIB[] = "Uninitialized library\0";
const char ACQUISITION_ERR_NO_DEVICE[] = "A compatible device could not be found or open\0";
const char ACQUISITION_ERR_CANNOT_CLAIM[] = "The device control cannot be claimed\0";
const char ACQUISITION_ERR_BAD_MALLOC[] = "The device control cannot be claimed\0";
const char ACQUISITION_ERR_UNKNOWN[] = "Unknown error\0";

#endif /* DEVICEACQUISITIONEXCEPTION_HPP */
