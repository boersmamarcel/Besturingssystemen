/* Necessary to prevent implicit declaration errors for sbrk */
#define _MINIX

#include <unistd.h>
#include <stdio.h>
#include "solution.h"

#define MEM_BLOCK_SIZE 1024*1024

/* Used for linked-list stack of empty memory spaces */
struct empty_space {
	task_t *this_space;
	struct empty_space *next_space;
};

/* Points to the start of the previous memory block */
struct space_start {
	char *ptr;
};

/* Points to the start of the last allocated memory block
 * startPointer always points to the address of a space_start
 * block for easy traversing of the different memory blocks
 */
char *startPointer;
task_t *topPointer;
int pointerSet = 0;
struct empty_space *lastempty;

void *task_alloc() {
	/* First time task_alloc() is called we need to do some initalization */
	if(!pointerSet) {
		struct space_start *start;
		
		startPointer = (char *)sbrk(MEM_BLOCK_SIZE);
		if((void *)startPointer == (void *)-1) {
			/* Handle the case where sbrk cannot allocate more memory */
			return NULL;
		}
		topPointer = (task_t*)startPointer;
		/* Place a pointer back to the previous memory block, which is
		 * non-existent in this case, so use NULL */
		start = (struct space_start *)topPointer;
		start->ptr = NULL;
		
		/* Increment pointer to first available memory block for a task */
		topPointer++;
		pointerSet = 1;
	}
	
	if(lastempty != NULL) { 	/* First try to reuse old memory blocks */
		void *result = lastempty->this_space;
		
		lastempty = lastempty->next_space;
		return result;
	}else if(topPointer - (task_t*)startPointer > MEM_BLOCK_SIZE / sizeof(task_t) - 1) {
		/* If there is no space for an additional element, allocate more memory
		 * Almost identical to code above, with the exception of start->ptr = result instead of NULL
		 */
		void *result = startPointer; /* temporarily use result to store the previous start pointer */
		struct space_start *start;
		
		startPointer = (char *)sbrk(MEM_BLOCK_SIZE);
		if((void *)startPointer == (void *)-1) {
			/* Handle the case where sbrk cannot allocate more memory */
			return NULL;
		}
		topPointer = (task_t*)startPointer;
		start = (struct space_start *)topPointer;
		start->ptr = result;
		topPointer++;
		result = topPointer;
		topPointer++;
		return result;
	}else{
		/* If memory is available, simple store the current value, increment the pointer and return the result */
		void *result = topPointer;
		topPointer++;
		return result;
	}
}

void task_free(void *ptr) {
	int valid = 0;
	char *curPointer = startPointer;
	
	/* Check to see if ptr is within one of the memory blocks
	 * Traverses through the memory blocks in a Last Allocated First Seen way
	 */
	while(!valid && curPointer != NULL) {
		if(ptr >= (void *)curPointer && (unsigned long)ptr < (unsigned long)curPointer+MEM_BLOCK_SIZE) {
			valid = 1;
		}else{
			curPointer = ((struct space_start *)curPointer)->ptr;
		}
	}
	
	/* If it is not a NULL pointer and the pointer is valid (i.e. within an allocated block) swap the task_t for an empty_space pointer struct */
	if(ptr != NULL && valid) {
		struct empty_space *newspace = ptr;
		
		newspace->this_space = ptr;
		newspace->next_space = lastempty;
		lastempty = newspace;
 		ptr = NULL;
	}
}