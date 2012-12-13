malloc calls sbrk (see brk(2)) to get more memory from the system when there is no suitable space already free.


sbrk allocates more memory to a program (if possible), and returns a pointer to the start of the new memory area


Steps to reproduce:

man malloc
man brk 2
find / -name malloc.c (according to the hints, malloc is implemented in a file called malloc.c )


The list of free slots is managed by a stack (first in, last out) via a linked-list implementation.

Please follow the comments in malloc.c.

Essentially, an orderded linked list of free slots is held in place of the userspace of the freed slots. When a slot of a certain size needs to be allocated, this linked list is checked for slots of at least this size. If find. the slot is split into two if the slot is bigger than the requested size, and then the linkedlist of free slots is joined together again. If no free slots are find, grow is called and a new free slot of BRKSIZE created, then in the second loop this free slot is found and used for the slot.
