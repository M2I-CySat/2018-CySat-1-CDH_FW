#ifndef _heap_h
#define _heap_h

struct heap_item {
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

#endif
