#include "as2_t1.h"
#include <stdio.h>
#include <stdlib.h>

void *task_alloc(void) {
	

	task_t * p;
	int i;

	if (counter < (MEM_BLOCK_SIZE / sizeof(task_t))) { 
		counter += 1;
	
		return (task_t *)get_MEM_BLOCK_START() + counter;
	} else {
		return NULL;
	}
}

void task_free(void *ptr) {
	if (!(ptr == NULL) && !((unsigned long)&ptr < (unsigned long)get_MEM_BLOCK_START())) {
		
	}	
} 
}
