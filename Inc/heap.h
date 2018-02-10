#ifndef _heap_h
#define _heap_h

#include <stdint.h>

enum heap_item_type {
	event,
	eps,
	payload,
	adcs
};

struct heap_item {
	uint32_t id;
	uint8_t prio;
	enum heap_item_type type;
	union {
		struct eps_data eps_data;
		struct payload_data payload_data;
		struct adcs_data adcs_data;
		struct event_data event_data;
	} data;
};

struct event_data {

};

struct eps_data {

};

struct payload_data {

};

struct adcs_data {

};

/* Allocate an item to be populated with data, then sent to the heap
 *
 * Only use for items that will be sent to the heap. For items to be
 * received from the heap, allocate memory some other way.
 * */
struct heap_item * Heap_AllocItem(void);

/* Free a heap item allocated with Heap_AllocItem (not normally used) */
void Heap_FreeItem(struct heap_item * item);

/* Send an item to the heap 
 *
 * If successful, memory ownership of the item will transfer to the
 * heap task. DO NOT free the item if PushItem is successful.
 * */
int Heap_PushItem(struct heap_item * item);

/* Retrieve an item from the heap. Memory must be managed by the calling
 * task. Do not use an item obtained with Heap_AllocItem.
 */
int Heap_PopItem(struct heap_item * out);

/* Initialize RTOS constructs */
int Heap_Init(void);

#endif
