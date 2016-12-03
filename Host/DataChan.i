%module DataChan

%begin %{
#pragma warning(disable:4100 4127 4706)
%}

%{
	#include "../CPP_API/DataChan.hpp"
	#include "../CPP_API/Device.hpp"
	#include "../CPP_API/DeviceAcquisitionException.hpp"
	#include "../CPP_API/InvalidMeasureException.hpp"
	#include "../CPP_API/Measure.hpp"
	#include "../CPP_API/NoDeviceException.hpp"
%}
%include "CPP_API/DataChan.hpp"
%include "CPP_API/Device.hpp"
%include "CPP_API/DeviceAcquisitionException.hpp"
%include "CPP_API/InvalidMeasureException.hpp"
%include "CPP_API/Measure.hpp"
%include "CPP_API/NoDeviceException.hpp"

