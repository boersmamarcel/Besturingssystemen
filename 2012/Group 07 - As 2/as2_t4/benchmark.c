#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include "solution.h"

#define LOOPS 1000*1000

int main(int argc, char **argv) {
	int i;
	task_t *tasks[LOOPS];
	struct timeval startTime, endTime;
	gettimeofday(&startTime, NULL);
	
	for(i = 0; i < LOOPS; i++) {
		tasks[i] = malloc(sizeof(task_t));
		/*tasks[i] = task_alloc();*/
	}
	
	for(i = 0; i < LOOPS; i++) {
		free(tasks[i]);
		/*tasks[i] = task_alloc();*/
	}
	
	gettimeofday(&endTime, NULL);
	
	printf("Time: %ld\n", (endTime.tv_usec-startTime.tv_usec));
	
	return 0;
}