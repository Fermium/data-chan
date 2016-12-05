# Getting Started

The library itself is really easy to use, you just need to remember to initialize the system at the beginning of your program and to shutdown the library before exiting :)

In order to have a *working* device you have to:

1. Initialize the library

1. Shutdown the library

You will be provided with code examples in the example directory of every API
binding:

| Dir       	| lang     	|
|------------	|---------	|
| API        	| C       	|
| CPP_API    	| C++     	|
| NODEJS_API 	| node.js 	|
| PYTHON_API 	| python  	|


## Initialize the library

The library initialization is __very__ important.

You __cannot__ acquire a device until you have called the initializer function.

Initializing the library setup the low-level libusb and possibly many others structures.


## Shutdown the library

The library shutdown importance is __unknown__.

Avoid library shutdown may lead to the host system in a bad state or with memory leaks: you __should__ never forget to shutdown the library.

Shutting down the library cleans memory that was used during the program execution.
