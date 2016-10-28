#include "../API/API.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void debug_measure(datachan_device_t *device) {
	uint8_t data_in[GENERIC_REPORT_SIZE];
	int data_size = datachan_raw_read(device, data_in);
	
	measure_t* data_out = (measure_t*)malloc(sizeof(measure_t));
	
	// the first byte is used to flag the presence of a measure
	uint8_t *data_pack = data_in;
	
	if ((data_size > 0) && (repack_measure(data_out, ++data_pack) == REPACK_SUCCESS)) {
		printf("\n\nMeasure: \n");
		printf("Value: %f", data_out->value);
	} else {
		printf("Bad measure.");
	}
	
	free((void*)data_out);
}

int main() {
	//initialize everything
	datachan_init();
    printf("Device transmission initialization success!\n\n");
	
	// try to acquire a device
	datachan_acquire_result_t scan_result = device_acquire();
	if (scan_result.result == success) {
		// get the device handler
		datachan_device_t *device = scan_result.device;
		printf("Target Device acquired\n");
		
		// enable data transmission
		if (datachan_device_enable(device))
			printf("\nDevice enabled\n\n");
		else
			printf("\nTransmission error!\n\n");
		
		sleep(1);
		
		// enable data transmission
		if (datachan_device_disable(device))
			printf("\nDevice disabled\n");
		else
			printf("\nTransmission error!\n");
		
		// release the device handler
		printf("\n\nreleasing the device...\n");
		device_release(&device);
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
