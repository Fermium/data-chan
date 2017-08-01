#include "../../DataChan.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv) {
    //initialize everything
    datachan_init();
    printf("Device transmission initialization success!\n\n");

    int vid, pid;
    do {
        printf("Enter Device PID:\n");
        scanf("%d", &pid);
        printf("Enter Device VID:\n");
        scanf("%d", &vid);
    } while ((vid > 0) && (pid > 0));

    // try to acquire a device
    datachan_acquire_result_t scan_result = datachan_device_acquire(vid, pid);
    if (scan_result.result == success) {
        // get the device handler
        datachan_device_t *device = scan_result.device;
        printf("Target Device acquired\n");

        // enable data transmission
        if (datachan_device_enable(device))
            printf("\nDevice enabled\n\n");
        else
            printf("\nTransmission error!\n\n");

        int k = 60;

        do {
            sleep(1);

            int i = datachan_device_enqueued_measures(device);
            printf("\n\n");

            while (i--) {
                measure_t* data_out = datachan_device_dequeue_measure(device);

                if (data_out != (measure_t*)NULL) {
                    for (int k = 0; k < data_out->measuresNum; k++) {
                      printf("Channel %u Value: %f              \r", (unsigned int)data_out->channels[k], data_out->values[k]);
                    }
                }

                free((void*)data_out);
            }
        } while (k > 0);

        // disable data transmission
        if (datachan_device_disable(device))
            printf("\nDevice disabled\n");
        else
            printf("\nTransmission error!\n");

        // release the device handler
        printf("\n\nreleasing the device...\n");
        datachan_device_release(device);
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
