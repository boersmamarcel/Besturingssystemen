#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int time;
    int maxIteration;
} Limit;

double pi = 0;
int iteration = 0;

void *printPiValue(void *params)
{

    Limit *values = params;

    while(1)
    {

        if(((clock()/CLOCKS_PER_SEC) > values->time) || iteration > values->maxIteration)
        {
            pthread_exit(0);
            break;
        }


        pthread_mutex_lock(&mutex);

        printf("Pi is currently: %.10f \n", 4*pi);

        pthread_mutex_unlock(&mutex);
        usleep(2000000);

    }
}



void *calculatePiValue(void *params){

    Limit *values = params;
    while(1)
    {
         if(((clock()/CLOCKS_PER_SEC) > values->time) || iteration > values->maxIteration)
        {
            /*stop the thread limit is reached*/
            printf("Limit reached done calculating\n");
            pthread_exit(0);
            break;
        }

        /*lock on object because we are going to change the value*/
        pthread_mutex_lock(&mutex);

        /*calculate pi value*/
        pi = pi + (pow(-1, iteration) / ((double)2*iteration + 1));

        /*unlock object for others*/
        pthread_mutex_unlock(&mutex);

        /*update the iteration count*/
        iteration = iteration + 1;
    }

}

int main(int argc, char* argv[])
{
    printf("Welcome to this awesome but however useless pi calculating program.\n");
    printf("Most people know that pi is something like 3.14... but we are going to calculate it for you :)\n");
    printf("And we are so awesome we do it with threads\n");

    /*create thread*/
    pthread_t calculation_thread[2];

    /*set limits (time and iteration)*/
    Limit params;
    if(argc  == 3)
    {
        params.time = atoi(argv[1]);
        params.maxIteration = atoi(argv[2]);
    }else if(argc == 1)
    {
        params.time= 10000000;
        params.maxIteration= 1000000000;
    }else{
        return 1;
    }

    pthread_attr_t attr;
    pthread_attr_init(&attr);


    /*create thread*/
    pthread_create(&calculation_thread[0], &attr, calculatePiValue,  &params);
    pthread_create(&calculation_thread[1], &attr, printPiValue,  &params);

    /*let the other thread join with the main thread*/
    pthread_join(calculation_thread[0], 0);
    pthread_join(calculation_thread[1], 0);

    return 0;
}

