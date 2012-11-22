/* Compile with gcc -Wall -O3 -o as1_t2 as1_t2.c dummysort.c
   Then, start ./as1_t2
*/
#include "as1_t2.h"
#include <stdlib.h>
#include <stdio.h>

void *flag(int i) {
	printf("Flag reached; %i\n", i);
	return NULL;
}

// Slicing function
task_t** slicer(task_t** tasks, int start, int end) {
	task_t** output = (task_t**) malloc(sizeof(unsigned long) * (end - start));
	
	int i;
	for(i = 0; i < (end - start); i++) {
		output[i] = (task_t*) malloc(sizeof(task_t));
		output[i]->id = tasks[start + i]->id;
	}
	return output;
}

task_t** merge(task_t** arr1, int length1, task_t** arr2, int length2) {
	task_t** output = (task_t**) malloc(sizeof(task_t*) * (length1 + length2));

	int i;
	int count1 = 0;
	int count2 = 0;

	for(i = 0; (count1 + count2)  < (length1 + length2); i++) {
		// Gimme some space
		output[i] = (task_t*) malloc(sizeof(task_t));

		if(count1 == length1) {
 			output[i]->id = arr2[count2]->id;
			count2++;
		}
		else if(count2 == length2) {
			output[i]->id = arr1[count1]->id;
			count1++;
		}
		else if(arr1[count1]->id > arr2[count2]->id) {
			output[i]->id = arr2[count2]->id;
			count2++;
		}
		else {
			output[i]->id = arr1[count1]->id;
			count1++;
		}
	}
	return output;
}

/** Dummy implementation for msort -- remove! */
task_t** sort(task_t** tasks, int count){
	if(count > 1) {
		int mid = count/2;

		// split the array in two
		task_t** arr1 = slicer(tasks, 0, mid);
		task_t** arr2 = slicer(tasks, mid, count);

		// sort each half
		arr1 = sort(arr1, mid);
		arr2 = sort(arr2, (count - mid));

		// merge the two halfs
		return merge(arr1, mid, arr2, (count - mid));
	}
	else {
		task_t** output = (task_t**) malloc(sizeof(task_t*));
		output[0] = (task_t*) malloc(sizeof(unsigned long));
		output[0] = tasks[0];
		return output;
	}
}

void msort(task_t** tasks, int count) {
	//qsort(tasks, count, sizeof(task_t*), qcomp);

	task_t** t = (task_t**) malloc(sizeof(task_t*) * count);
	t = sort(tasks, count);

	int i;

	for(i = 0; i < count; i++) {
		tasks[i] = (task_t*) malloc(sizeof(task_t));
		tasks[i] = t[i];
	}

}
