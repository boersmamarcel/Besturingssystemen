Our solution is in solution.c

There is pointer to the next pointer in the allocated memory block. In task_alloc, a check is made (on line 28) to see if there is another slot available for a task_t structure. If that is not the case, the function returns NULL (the same value malloc returns when no memory block could be allocated). If there is a free slot, the address of the next slot is returned and the pointer is incremented.

When there is a free slot, as signified by a non-null value of "lastempty", that slot is used, and lastempty is set to the next free slot, or NULL if not available. The implementation is therefor a linked-list of free slots.

task_free() checks if the pointer is within the allocated memory block (line 41), then creates a pointer to the previous free block and places it within the memory slot (lines 43-46).

Flowcharts:
-task_free : task_free.png
-task_alloc : task_alloc.png