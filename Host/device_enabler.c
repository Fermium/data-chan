/**
	
	Copyright (C) 2017 Fermium LABS srl

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../config.h"
#include "DataChan.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>

bool datachan_device_is_enabled(datachan_device_t* dev) {
    if (dev == (datachan_device_t*)NULL)
        return false;

    bool enabled;

    pthread_mutex_lock(&dev->enabled_mutex);
    enabled = dev->enabled;
    pthread_mutex_unlock(&dev->enabled_mutex);

    return enabled;
}

bool datachan_device_disable(datachan_device_t* dev) {
    if (dev == (datachan_device_t*)NULL)
        return false;

    bool enabled = datachan_device_is_enabled(dev);

    if (enabled) {
        // write the DISABLE_TRANSMISSION command on the USB bus to disable the device
        datachan_send_sync_command(dev, DISABLE_TRANSMISSION, (uint8_t*)NULL, 0);
        
        // if this point is reached the communication will fall in few millis
        enabled = false;

        // the next call to is_enabled will fail (even on the thread)
        pthread_mutex_lock(&dev->enabled_mutex);
        dev->enabled = enabled;
        pthread_mutex_unlock(&dev->enabled_mutex);

        // so... let's just wait for the thread to gracefully stop
        void *data = NULL;
        pthread_join((pthread_t)dev->reader, &data);
    }

    // is the device enabled now?
    return !enabled;
}

void* IO_bulk_thread(void* device);
bool datachan_device_enable(datachan_device_t* dev) {
    if (dev == (datachan_device_t*)NULL)
        return false;

    bool enabled = datachan_device_is_enabled(dev);

    if (!enabled) {
        // write the ENABLE_TRANSMISSION command on the USB bus to enable the device
        datachan_send_sync_command(dev, ENABLE_TRANSMISSION, (uint8_t*)NULL, 0);

        // report the result
        enabled = (pthread_create(
                (pthread_t *)&dev->reader,
                (const pthread_attr_t *)&dev->reader_attr,
                &IO_bulk_thread,
                (void*)dev
            ) == 0);

        // save the status
        pthread_mutex_lock(&dev->enabled_mutex);
        dev->enabled = enabled;
        pthread_mutex_unlock(&dev->enabled_mutex);
    }

    // is the device enabled now?
    return enabled;
}
