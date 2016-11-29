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

#include "DeviceAcquisitionException.hpp"

using namespace DataChan;

DeviceAcquisitionException::DeviceAcquisitionException(const char* err_str) {
    // store the pointer to the message
    this->errStr = err_str;
}

const char* DeviceAcquisitionException::what() noexcept {
    return this->errStr;
}