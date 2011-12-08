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


/**
 * This function sorts the tasks array by using the merge sort algorithm
 *
 * @require (sizeof(tasks) / sizeof(task[i]))
 * @ensure  for each int i in [0..count-1] 
 *            tasks[i]->id < tasks[i+1]->id
 */
void msort(task_t** tasks, int count)
{
  //printf("%p", tasks);

  if (count <= 1) {
    // Nothing happens here, because when there is only one or if there are zero elements in the array the array is already sorted.
  } else {
    // We need to calculate the middle of the tasks because we want to split the tasks into two halves		
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
 * Function which merges two (virtual) sorted task array, they are virtual because it is just the main array with bounds on left, mid and right
 * Everything from left to mid should be sorted
 * Everything from mid to right should be sorted 
 *
 * @require for each int i in [left..mid-1]
 *            tasks[i]->id < tasks[i+1]->id
 *          for each int j in [mid..right-1]
 *            tasks[j]->id < tasks[i+1]->id
 * @ensure  for each int k in [left..right-1]
              tasks[k]->id < tasks[k+1]->id
 */
void merge(task_t** tasks, int left, int mid, int right) {
  
  int leftCounter = left;
  int rightCounter = mid;

  int resultCounter = 0;

  task_t** result;
  result = (task_t**) malloc(sizeof(task_t*) * (right-left));
  
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

