#include "../API/API.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    //initialize everything
    datachan_init();
    printf("Device transmission initialization success!\n\n");
	
    // try to acquire a device
    datachan_acquire_result_t scan_result = datachan_device_acquire();
    if (scan_result.result == success) {
        // get the device handler
        datachan_device_t *device = scan_result.device;
        printf("Target Device acquired\n");

        // enable data transmission
        if (datachan_device_enable(device))
            printf("\nDevice enabled\n\n");
        else
            printf("\nTransmission error!\n\n");

        // sleep one second
        sleep(1);
        
        // enable data transmission
        if (datachan_device_disable(device))
            printf("\nDevice disabled\n");
        else
            printf("\nTransmission error!\n");
	
        int i = datachan_device_enqueued_measures(device);
        printf("Read %d measures in about 1 second", i);
        while (i--) {
            measure_t* data_out = datachan_device_dequeue_measure(device);
            
            if (data_out != (measure_t*)NULL) {
                    printf("\n\nMeasure: \n");
                    printf("Value: %f", data_out->value);
            }
            
            free((void*)data_out);
        }
        
        // release the device handler
        printf("\n\nreleasing the device...\n");
        datachan_device_release(&device);
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
