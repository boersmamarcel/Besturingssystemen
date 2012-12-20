#include <unistd.h>
#include <stdio.h>
#include "as2_t1.h"

/* Used for linked-list stack of empty memory spaces */
struct empty_space {
	task_t *this_space;
	struct empty_space *next_space;
};

/* Points to the next available memory slot (not accounting for empty slots due to freeing) */
task_t *topPointer;
int pointerSet = 0;
struct empty_space *lastempty;

void *task_alloc() {
	/* Do some initialization the first time */
	if(!pointerSet) {
		topPointer = get_MEM_BLOCK_START();
		pointerSet = 1;
	}
	
	/* If there is a previous released memory slot, use it */
	if(lastempty != NULL) {
		void *result = lastempty->this_space;
		lastempty = lastempty->next_space;
		return result;
	}else if(topPointer - (task_t*)get_MEM_BLOCK_START() > MEM_BLOCK_SIZE / sizeof(task_t) - 1) {
		/* If no more memory is available, return a NULL pointer, similar to how malloc handles this */
		return NULL;
	}else{
		/* If all goes well we can just return the topPointer and increment it */
		void *result = topPointer;
		topPointer++;
		return result;
	}
}

void task_free(void *ptr) {
	/* Check to see if the ptr is within our memory block */
	if(ptr != NULL && ptr >= get_MEM_BLOCK_START() && (unsigned long)ptr < (unsigned long)get_MEM_BLOCK_START()+MEM_BLOCK_SIZE) {
		/* If so, create a new empty_space struct to store in the memory slot of the free'd task */
		struct empty_space *newspace = ptr;
		newspace->this_space = ptr;
		newspace->next_space = lastempty;
		lastempty = newspace;
	}
}