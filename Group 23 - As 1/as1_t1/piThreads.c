#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

/*For mutex lock*/
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

long double pi = 0; /*This is shared info between threads...because they share resources*/
void *piCalculator(void *param); /* the calculating thread */
void *piPrinter(void *param); /*printing thread*/
int maxIterations;
int maxExecTime;
clock_t start, end;
int iteration = 0;

int main(int argc, char* argv[])
{
	pthread_t      worker_thread, printer_thread;

	if(argc != 3) {
		printf("Usage: %s <max iterations> <max execution time>\n", argv[0]);
		return 0;
	}

	maxIterations = atoi(argv[1]);
	maxExecTime   = atoi(argv[2]) - 1;

	start = clock();

	pthread_create( &worker_thread,  NULL, piCalculator, NULL);
	pthread_create( &printer_thread, NULL, piPrinter,    NULL);
	// now we wait...
	pthread_join(worker_thread, NULL);
	pthread_mutex_destroy( &mutex );

	end = clock();

	printf("\nexecution time = %.2fs\n", (((double)end)-((double)start))/CLOCKS_PER_SEC);
	printf("iterations     = %d\n", iteration);
	printf("pi             = %1.60Lf\n", 4*pi);

	return 0;
}

/*The printing thread*/
void *piPrinter(void *param)
{
	while(1) {
		printf("Iteration (%10d): %1.60Lf\n", iteration, 4*pi);
		sleep(1);
	}
}

/*The thread will begin in this function*/
void *piCalculator(void *param)
{
	while(iteration < maxIterations)
	{
		if( ((clock()-start)/CLOCKS_PER_SEC) > maxExecTime ) {
			printf("Time limit reached!\n");
			pthread_exit(NULL);
		}

		/*Thread locks on mutex1 */
		pthread_mutex_lock( &mutex );
		/*calculate pi */
		pi = pi + ( pow(-1, iteration)/(2*iteration+1) );
		/*Done calculating so unlock on mutex1*/
		pthread_mutex_unlock( &mutex );

		iteration++;
	}

	printf("Reached iteration limit!\n");
	pthread_exit(NULL);
}
