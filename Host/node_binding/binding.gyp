{
  "targets": [
    {
      "target_name": "DataChan",
      "sources": [ "../CPP_API/Device.cpp", "../CPP_API/DeviceAcquisitionException.cpp", "../CPP_API/InvalidMeasureException.cpp", "../CPP_API/Measure.cpp","../CPP_API/NoDeviceException.cpp", "./DataChan_wrap.cxx" ],

	"cflags!": [ "-fno-exceptions" ],
    "cflags_cc!": [ "-fno-exceptions" ],
	"include_dirs" : [
    		"<!(node -e \"require('nan')\")"
		]
    }
  ]
}
