#include "../API/API.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
	//initialize everything
	datachan_init();
    printf("Device transmission initialization success!\n\n");
	
	// try to acquire a device
	datachan_acquire_result_t scan_result = acquire_device();
	if (scan_result.result == success) {
		// get the device handler
		datachan_device_t *device = scan_result.device;
		printf("Target Device acquired\n");
		
		// release the device handler
		release_device(&device);
	} else {
		printf("Something went wrong :(\n\n");
		
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
	}
	
	// shutdown everything
	datachan_shutdown();
	printf("\nDevice transmission shutdown\n");
	return EXIT_SUCCESS;
}
