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

#ifndef DATACHAN_H
#define DATACHAN_H

#ifdef _MSC_VER
  #define _CRT_SECURE_NO_WARNINGS 1
#endif

#if _MSC_VER
	//disable warnings on dll interface unexported
	#pragma warning (disable : 4275)
	//disable warnings on dll interface unexported
	#pragma warning (disable : 4251)

	//disable warnings on incomplete type sqlite3
	#pragma warning (disable : 4150)
	//disable warnings on 255 char debug symbols
	#pragma warning (disable : 4786)
	//disable warnings on extern before template instantiation
	#pragma warning (disable : 4231)
#endif


#ifdef _WIN32
	#ifdef DATACHAN_CPP_EXPORT
		#define DATACHAN_CPP_API __declspec(dllexport)
	#else
		#define DATACHAN_CPP_API __declspec(dllimport)
	#endif

#else
        #define DATACHAN_CPP_API
#endif

DATACHAN_CPP_API class DataChan {
public:
    static void Init();
    static void Shutdown();
    static bool Initialized();
    DataChan();
    virtual ~DataChan();
private:

};

#endif /* DATACHAN_H */

