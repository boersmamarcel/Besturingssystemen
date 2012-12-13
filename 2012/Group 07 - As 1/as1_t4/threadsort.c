/* Compile with 
	gcc -Wall -O3 -o as1_t2 as1_t2.c threadsort.c
   Then, start ./as1_t2
*/
#include "as1_t2.h"
#include "threadsort.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

// Max number of threads
#define MAX_THREAD_COUNT 4

// // Thread pool (not used now)
// pthread_t thread_pool[MAX_THREAD_COUNT];
int thread_count = 0;
/* Mutex lock */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* struct for communicating the limits to the calculation thread */
typedef struct {
    task_t **tasks;
    int left;
    int right;
} merge_args;

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

void *merge_thread_sort(void* params) {
	merge_args *args = params;
	merge_sort(args->tasks, args->left, args->right);
	pthread_mutex_lock(&mutex);
	thread_count--;
	pthread_mutex_unlock(&mutex);
	// printf("Killing thread. Current count: %d \n", thread_count);
	pthread_exit(0);
}

void merge_sort(task_t **tasks, int left, int right) {
	pthread_t left_thread;
	int thread_created = 0;
	// Only split up when two or more elements
	if(left >= 2) {
		int newleft = left/2;
		int newright = left-newleft;
		if (thread_count < MAX_THREAD_COUNT) {
			pthread_mutex_lock(&mutex);
			// Look if still under the thread limit
			if (thread_count < MAX_THREAD_COUNT) { 
				thread_count++;
				pthread_mutex_unlock(&mutex);
				// Left gets a new thread
				merge_args args;
				args.tasks = tasks;
				args.left = newleft;
				args.right = newright;

				// printf("Creating new thread! Current count: %d \n", thread_count);
				thread_created = 1;
				int thread_result = pthread_create(&left_thread, NULL, merge_thread_sort, &args);
				
				//ERROR HANDLING
				if (thread_result == EAGAIN) {
					printf("THREAD ERROR: %d, NO RESOURCES LEFT IN SYSTEM",thread_result);
					// exit(127);
					thread_created = 0;
				} else if (thread_result == EINVAL) {
					printf("THREAD ERROR: %d, EINVAL",thread_result);
					exit(127);
				} else if (thread_result == EPERM) {
					printf("THREAD ERROR: %d, EPERM",thread_result);
					exit(127);
				}
			} else {
				pthread_mutex_unlock(&mutex);
			}
		}
		if(thread_created == 0) {
			// pthread_mutex_unlock(&mutex);
			// printf("Max thread reached, do it yourself! Current count: %d \n", thread_count);
			
			merge_sort(tasks, newleft, newright);
		}
	}
	
	// Only split up when two or more elements
	if(right >= 2) {
		int newleft = right/2;
		int newright = right-newleft;
		// Change the pointer to a new element within the array (tasks+left)
		merge_sort(tasks+left, newleft, newright);
	}
	// Wait for left thread to finish
	if (thread_created)
		pthread_join(left_thread, NULL);

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