#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include "solution.h"

#define LOOPS 1000*1000

void *printResult(struct timeval start, struct timeval end) {
	int result;
	result = end.tv_usec - start.tv_usec;
	   
	if(result < 0) /* Protect against "underflows" */
		result += 1000000; /* Add one second */
	
	/* Print the result */
	printf("Time: %ld microsec\n\n", result);
}

int main(int argc, char **argv) {
	int i; /* Our counter */
	task_t *tasks[LOOPS]; /* Array of rubbish to test with */
	struct timeval startTime1, endTime1; /* Clocks (could have been re-used */
	struct timeval startTime2, endTime2;

	/* Testing Malloc, for reference */
	printf("Benchmarking Malloc: \n");
	gettimeofday(&startTime1, NULL);
	for(i = 0; i < LOOPS; i++) {
		tasks[i] = malloc(sizeof(task_t));
	}
	for(i = 0; i < LOOPS; i++) {
		free(tasks[i]);
	}
	gettimeofday(&endTime1, NULL);
	printResult(startTime1, endTime1);

	/* Testing our solution; see how well we did */
	printf("Benchmarking our solution: \n"); 
	gettimeofday(&startTime2, NULL);
	for(i = 0; i < LOOPS; i++) {
		tasks[i] = task_alloc();
	}
	for(i = 0; i < LOOPS; i++) {
		task_free(tasks[i]);
	}
	gettimeofday(&endTime2, NULL);
	printResult(startTime2, endTime2);

	return 0;
	/* Program terminated... */
}
