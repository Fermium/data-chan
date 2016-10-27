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
		
		uint8_t data_in[GENERIC_REPORT_SIZE];
		int data_size = datachan_raw_read(device, data_in);
		
		if (data_size > 0) {
			printf("Data received via interrupt transfer %02x bytes:\n", data_size);
			int i;
		  	for(i = 0; i < data_size; i++)
		  		printf("%02x ",data_in[i]);
		} else {
			printf("No data received");
		}
		
		// enable data transmission
		printf("\nTransmission of enable signal...\n");
		uint8_t cmd[] = { CMD_MAGIC_FLAG, 0x01 };
		data_size = datachan_raw_write(device, cmd, 2);
		if (data_size > 0)
			printf("\nCommand sent!\n");
		else
			printf("\nTransmission error!\n");
		
		// test the first measure
		data_size = datachan_raw_read(device, data_in);
		if (data_size > 0) {
			printf("Data received via interrupt transfer %02x bytes:\n", data_size);
			int i;
		  	for(i = 0; i < data_size; i++)
		  		printf("%02x ",data_in[i]);
		} else {
			printf("No data received");
		}
		
		// release the device handler
		printf("\nreleasing the device...\n");
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
