#include "../API/API.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
	datachan_init();
    printf("Initialization success!\n\n");
	
	datachan_acquire_result_t scan_result = acquire_device();
	if (scan_result.result == success) {
		printf("Target acquired\n");
	} else {
		printf("Something went wrong :(\n\n");
		
		switch (scan_result.result) {
			case uninitialized:
				printf("Uninitialized library");
				break;
			
			case not_found_or_inaccessible:
				printf("A compatible device could not be found or open");
				break;
				
			case cannot_config:
				printf("The device communication protocol could not be configured");
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
	
	printf("\n");
	return EXIT_SUCCESS;
}
