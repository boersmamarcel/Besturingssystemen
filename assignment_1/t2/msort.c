#include <stdio.h>
#include <stdlib.h>
#include "as1_t2.h"
#include "math.h"

void debug_tasks(task_t** tasks, int count);
void merge(task_t** tasks, int left, int mid, int right);
void msort(task_t** tasks, int count);
task_t** slice_tasks(task_t** tasks, int begin, int end);

void debug_tasks(task_t** tasks, int count) {
  printf("tasks: [");
  int i;
  for(i = 0; i<count; i++) {
    printf("%lu:", tasks[i]->id);
    // printf("%p", tasks[i]);
    if (i+1 != count) {
      printf(",");
    }
  }
  printf("]\n");
}



/*int main(int argc, char* argv[])
{
  task_t** randomTasks = makeTasks(makeRandomIds(20), 20);
  msort(randomTasks, 20);
  
  int i;
  for(i=0; i<20; i++) {
    printf("elem: %lui", randomTasks[i]->id);
  }

  return 0;
}*/

void msort(task_t** tasks, int count)
{
  // printf("\t\t\tmergesort-start count: %i ", count);
  // debug_tasks(tasks, count);

  // This is the end step, when there is only one or zero elements nothing should happen to the tasks passed in.
	if (count <= 1) {
    // Nothing happens
  } else {
    int mid = floor(count / 2);
    
    task_t** left =  slice_tasks(tasks, 0, mid);
    msort(left, mid-0);    

    task_t** right = slice_tasks(tasks, mid, count);
    msort(right, count-mid);

    // debug_tasks(left, mid-0);
    // debug_tasks(right, count-mid);

    int i = 0;
    for(;i < count;i++) {
      if(i < mid) {
        tasks[i] = left[i];
      } else {
        tasks[i] = right[i-mid];
      }
    }

    free(left);
    free(right);

    merge(tasks, 0, mid, count);

    // debug_tasks(tasks, count);
    // printf("!  taskId: %lu \n", tasks[mid]->id);
    
    //printf("\t\t\tmergesort-end count: %i ", count);
    //debug_tasks(tasks, count);
    //printf("\n");
  }
}

/**
 * Function to merge the results of a mergesort, merges
 */
void merge(task_t** tasks, int left, int mid, int right) {
  
  int leftCounter = left;
  int rightCounter = mid;

  int resultCounter = 0;

  task_t** result;
  result = (task_t**) malloc(sizeof(task_t*) * (right-left));
  
  for(resultCounter = 0; resultCounter < (right-left); resultCounter++){
    // printf("loop\n");    

    if (leftCounter < mid && rightCounter < right)
    {
      if(tasks[leftCounter]->id <= tasks[rightCounter]->id)
      {
        result[resultCounter] = tasks[leftCounter];
        leftCounter++;
      }else{
        result[resultCounter] = tasks[rightCounter];
        rightCounter++;
      }
    }else if(leftCounter < mid){
      result[resultCounter] = tasks[leftCounter];
      leftCounter++;
    }else if(rightCounter < right){
      result[resultCounter] = tasks[rightCounter];
      rightCounter++;
    }

  }


  int i = 0;

  for(; i < resultCounter; i++)
  {
    tasks[i+left] = result[i];
  }

  free(result);

}

/**
 * Slices an array to get only a part of it
 * @require end < sizeof(array)/sizeof(tasks) && begin > 0 && begin < end
 */
task_t** slice_tasks(task_t** tasks, int begin, int end) {
  int i;
  task_t** result;
  result = (task_t**) malloc(sizeof(task_t*) * (end-begin));
  for (i = 0; i < (end - begin); i++) {
    result[i] = tasks[i+begin];
    // printf("* task-pointer: %p\n", result[i]);
    // printf("* value: %lu\n", result[i]->id);
  }
  return result;
}

