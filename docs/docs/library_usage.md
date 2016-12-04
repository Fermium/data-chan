# Getting Started
This page provides you with everything you need to know in order to work with
__every__ API library.

## Basic software flow
The bare minimum you may want to do is getting some measures from your pretty device, right?

Well, in order to to that you will need to:

1. Initialize the library
1. Acquire the device
1. Check for errors
1. Do stuff
1. If no errors occurred, release the device
1. Shutdown the library

Using C API you would do something like this:

```c
#include <API.h>

int main(int argc, char** argv) {
    // Initialize the library
    datachan_init();
    
    // Acquire the device
    datachan_acquire_result_t scan_result = datachan_device_acquire();

    // Check for acquire result
    if (scan_result.result == success) {
        // get the device handler
        datachan_device_t *device = scan_result.device;
		
        // enable data transmission
        if (datachan_device_enable(device))
            printf("\nDevice enabled\n\n");
        else
            printf("\nTransmission error!\n\n");
		
		// Do Stuff :)
		
        // Disable data transmission
        if (datachan_device_disable(device))
            printf("\nDevice disabled\n");
        else
            printf("\nTransmission error!\n");	
		
        // Release the device
        datachan_device_release(dev);
    } else {
		switch (scan_result.result) {
            case uninitialized:
                printf("Uninitialized library");
                break;

            case not_found_or_inaccessible:
                printf("A compatible device could not be found or open");
                break;

            case cannot_claim:
                printf("The device control cannot be claimed");
                break;

            case malloc_fail:
                printf("Memory allocation error");
                break;

            default:
                printf("Unknown error");
    }
	
	// Shutdown the library
	datachan_shutdown();

	return 0;
}
```
