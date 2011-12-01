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
    if (i+1 != count) {
      printf(",");
    }
  }
  printf("]\n");
}

void msort(task_t** tasks, int count)
{

  if (count <= 1) {
    // Nothing happens here, because ..
  } else {
    int mid = floor(count / 2);
    
    task_t** left =  slice_tasks(tasks, 0, mid);
    msort(left, mid-0);    

    task_t** right = slice_tasks(tasks, mid, count);
    msort(right, count-mid);

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
  result = (taskd_t**) malloc(sizeof(task_t*) * (right-left));
  
  for(resultCounter = 0; resultCounter < (right-left); resultCounter++){
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
  }
  return result;
}

