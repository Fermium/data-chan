/**
	data-chan physic through USB
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

#include <stdlib.h>
#include <string.h>
#include "Settings.h"

static struct setting_queue_t *settings_queue = NULL;
static memory_block_t value;

memory_block_t* getSetting(uint32_t entry, uint8_t channel) {
	bool found = false, foundFailback = false;

	// iterate over all elements to find the correct one
	struct setting_queue_t *current = settings_queue;

	// search the setting for the given channel
	while ((current != (struct setting_queue_t*)NULL) && (!found)) {
		if ((current->entry->entry == entry) && (current->entry->channel == channel)) {
			memcpy((void*)&value, (const void*)&current->entry->setting, sizeof(memory_block_t));
			found = true;
		}
		// search the setting for the global channel (as failback)
		else if ((current->entry->entry == entry) && (current->entry->channel == 0)) {
			memcpy((void*)&value, (const void*)&current->entry->setting, sizeof(memory_block_t));
			foundFailback = true;
		}

		// head for the next entry
		current = current->next;
	}

	// return the setting if found
	if ((found) || (foundFailback)) return &value;

	// by default NULL is returned
	return (memory_block_t*)NULL;
}

void setSetting(const setting_entry_t *setting) {
	// create memory for the new entry and copy the setting
	setting_entry_t *e = (setting_entry_t*)malloc(sizeof(setting_entry_t));
	memcpy((void*)e, (const void*)setting, sizeof(setting_entry_t));

	if (settings_queue == (struct setting_queue_t*)NULL) {
		// initialize the queue
		settings_queue = (struct setting_queue_t*)malloc(sizeof(struct setting_queue_t));
		settings_queue->next = (struct setting_queue_t*)NULL;
		settings_queue->entry = e;
	} else {
		// iterate over all elements to find the correct one, or create a new one in the very end of the list
		struct setting_queue_t *current = settings_queue;

		// try to update an existing setting
		bool inserted = false;
		while ((current->next != (struct setting_queue_t*)NULL) && (!inserted)) {
			if ((current->entry->entry == setting->entry) && (current->entry->channel == setting->channel)) {
			    // remove the memory for the old setting
                free(current->entry->setting.start_addr);

                // save the new setting
                current->entry->setting = setting->setting;
				inserted = true;

				// avoid memory leak
				free((void*)e);
			}
		}

		// if it was not possible create a new one
		if (!inserted) {
			current->next = (struct setting_queue_t*)malloc(sizeof(struct setting_queue_t));
			current = current->next;

			current->entry = e;
			current->next = (struct setting_queue_t*)NULL;
		}
	}
}
