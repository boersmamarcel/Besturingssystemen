#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>

/*For mutex lock*/
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

long double pi = 0; /*This is shared info between threads...because they share resources*/
void *piCalculator(void *param); /* the calculating thread */
void *piPrinter(void *param); /*printing thread*/

int main(int argc, char* argv[])
{
	printf("Welcome to the awesome pi calculator!!!\n");
	printf("We'll do some calculating pi and displaying results in several threads.\n\n");
	
	
	pthread_t calculate_pi_thread, print_pi_value_thread; /*Thread id*/
	pthread_attr_t attr; /*Thread attributes*/

	pthread_attr_init(&attr); /*Set the attributes for the thread*/
	pthread_create(&calculate_pi_thread, &attr, piCalculator, NULL);
	pthread_create(&print_pi_value_thread, &attr, piPrinter, NULL);

	
	pthread_join(calculate_pi_thread, NULL);
	pthread_join(print_pi_value_thread, NULL);	

	printf("Done pi is calculated!!!\n\n");


	return 1;

}

/*The printing thread*/
void *piPrinter(void *param)
{
	bool print_value = true;
	
	while(print_value){
		printf("The current value of pi :  %1.65Lf\n", 4*pi);
		sleep(1);
	}
	/*Sleep for some milli seconds*/
	pthread_exit(0);
}

/*The thread will begin in this function*/
void *piCalculator(void *param)
{
	int i = 0;
	while(true)
	{
		/*Thread locks on mutex1 */
		pthread_mutex_lock( &mutex1 );
		/*calculate pi/4 */
		pi = pi + ( pow(-1, i)/(2*i+1) );
		/*Done calculating so unlock on mutex1*/
		pthread_mutex_unlock( &mutex1 );	
		i++;	
	}

	pthread_exit(0);
}
