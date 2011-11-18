#include "as2_t1.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node {
	struct node* next;
} node;

int initialized = 0;
node * prev;
node * curr;
node * head;

void *task_alloc(void) {

	printf("HIHIHIHIHIHIHIHIHI\n");
	
	if (initialized == 0) {
		printf("HI DOES THIS HAPPEN AT ALL\n");
		curr = (node *)((unsigned int)get_MEM_BLOCK_START() + MEM_BLOCK_SIZE - sizeof(task_t));
		prev = NULL;
		curr->next = (node *)((unsigned int)get_MEM_BLOCK_START());

		printf("I have a small hunch this doesn't fuck things up\n");
	
		for (; (unsigned int)curr >= (unsigned int)get_MEM_BLOCK_START() + 2*sizeof(task_t); prev=curr, curr = (node *)((unsigned int)curr - sizeof(task_t))) {
			/* printf("%p \n", main_pointer); */
/*		curr->next = (node *)((unsigned int)get_MEM_BLOCK_START());*/	
		curr->next = prev;
/*	printf("%p %p %p\n", curr, curr->next, prev);*/
			/*curr->next = (node *)prev;*/
/*			printf("%p -> %p\n", head, head->next);*/ 
		}

		printf("Finished looping yo \n");

		printf("HI HIHIHIHIHI\n");
		head = (node *)((unsigned int)get_MEM_BLOCK_START());
	
		printf("Einde: %p\n", head);	
		initialized = 1;
	} 

	printf("%p \n", curr);
	printf("%p \n", head);

	
	curr = head->next;
	/*if ((unsigned int)curr == (unsigned int)NULL) {
		head->next = curr->next;
	}*/

	return curr;		
}

void task_free(void *ptr) {
	if (!(ptr == NULL) && !((unsigned long)&ptr < (unsigned long)get_MEM_BLOCK_START())) {
		
	}	
} 

