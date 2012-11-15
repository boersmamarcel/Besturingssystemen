#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#define MICROCONVERT 1000000
#define CALCULATION_COMPLETED 1
#define CALCULATION_NOT_COMPLETED 0

/* struct for communicating the limits to the calculation thread */
typedef struct {
    int iterations;
	int time;
} limits;

/* Mutex lock */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

double pi;
int iteration = 0;
int completed = CALCULATION_NOT_COMPLETED;

void *piCalculation(void *params) {
	
	limits *my_limit = params;
	
	while(1) {
		pthread_mutex_lock(&mutex);
		
		/* Check for limits, if iterations and time are 0 it should run indefinitely */
		if((iteration >= my_limit->iterations || clock()/CLOCKS_PER_SEC > my_limit->time) && (my_limit->iterations != 0 && my_limit->time != 0)) {
			
			/* Mark as completed for the print thread */
			completed = CALCULATION_COMPLETED;
			
			pthread_mutex_unlock(&mutex);
			pthread_exit(0);
		}
		
		/* Leibniz pi calculation */
		pi += pow(-1.0, iteration) / ((double)2*iteration+1);
		iteration++;
		
		pthread_mutex_unlock(&mutex);
	}
	
	return NULL;
}

void *piPrinting(void *params) {
	
	while(1) {
		pthread_mutex_lock(&mutex);
		/* Check for thread completion */
		if(completed == CALCULATION_COMPLETED) {
			printf("Iteration: %20d %.15f\n", iteration, pi*4);
			printf("Done!\n");
			pthread_mutex_unlock(&mutex);
			pthread_exit(0);
		}
		printf("Iteration: %20d %.15f\n", iteration, pi*4);
		pthread_mutex_unlock(&mutex);
		
		/* Wait until next print */
		usleep(2*MICROCONVERT);
	}
	
	return NULL;
}

int main(int argc, char *argv[]) {
	
	pthread_t calculation_thread;
	pthread_t print_thread;
	
	limits calc_limit;
	
	/* Check for command line input */
	if(argc == 1) {
		calc_limit.iterations = 0;
		calc_limit.time = 0;
	}else if(argc == 3) {
		calc_limit.time = atoi(argv[1]);
		calc_limit.iterations = atoi(argv[2]);
	}else {
		printf("Usage: %s\n\tLoops indefinitely\n", argv[0]);
		printf("Usage: %s time iterations\n\tLimits to time seconds or iterations\n", argv[0]);
		return 1;
	}
	
	/* Create threads */
	pthread_create(&calculation_thread, NULL, piCalculation, &calc_limit);
	pthread_create(&print_thread, NULL, piPrinting, NULL);
	
	/* Wait for calculation and printing threads to finish */
	pthread_join(calculation_thread, NULL);
	pthread_join(print_thread, NULL);
	
	return 0;
}