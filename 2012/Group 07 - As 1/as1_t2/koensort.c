/* Compile with gcc -Wall -O3 -o as1_t2 as1_t2.c koensort.c
   Then, start ./as1_t2
*/
#include "as1_t2.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void merge(task_t **tasks, int left, int right) {
	
	// Initialize new temporary array
	task_t **temp = malloc(sizeof(task_t*)*(left+right));
	int cur_left = 0;
	int cur_right = 0;
	
	// Loop through elements while there are elements left
	while(cur_left+cur_right < left+right) {
		
		if(cur_left == left) { // All elements from left set processed?
			temp[cur_left+cur_right] = tasks[left+cur_right];
			cur_right++;
		}else if(cur_right == right) { // All elements from right set processed?
			temp[cur_left+cur_right] = tasks[cur_left];
			cur_left++;
		}else{
			// Both left and right sets have elements left. Which one to pick?
			if(tasks[cur_left]->id < tasks[left+cur_right]->id) {
				temp[cur_left+cur_right] = tasks[cur_left];
				cur_left++;
			}else{
				temp[cur_left+cur_right] = tasks[left+cur_right];
				cur_right++;
			}
		}
	}

	// Switch around the pointers in the array
	int i;
	for(i = 0; i < left+right; i++) {
		tasks[i] = temp[i];
	}
}

void merge_sort(task_t **tasks, int left, int right) {
	
	// Only split up when more than two elements
	if(left >= 2) {
		int newleft = left/2;
		int newright = left-newleft;
		merge_sort(tasks, newleft, newright);
	}
	
	// Only split up when more than two elements
	if(right >= 2) {
		int newleft = right/2;
		int newright = right-newleft;
		// Change the pointer to a new element within the array (tasks+left)
		merge_sort(tasks+left, newleft, newright);
	}
	
	// Merge!
	merge(tasks, left, right);
}

void msort(task_t** tasks, int count) {
	
	// When count <= 1 the array is already sorted
	if(count >= 2) {
		int left = count/2;
		int right = count-left;
		merge_sort(tasks, left, right);
	}
	
}