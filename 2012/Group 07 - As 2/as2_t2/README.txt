Steps to reproduce:
man malloc
man brk 2
find / -name malloc.c (according to the hints, malloc is implemented in a file called malloc.c )


The list of free slots is managed by a stack (first in, last out) via a linked-list implementation.

Please follow the comments in malloc.c, as they should explain the code.

To briefly summarize the flow of the program: an orderded linked list of free slots is held in place of the userspace of the freed slots. 
When a slot of a certain size needs to be allocated, this linked list is checked for slots of at least this size. If the slot is bigger than the requested size, it is split in two and the linkedlists of free slots is joined together again. If no free slots are found, a grow function is called and a new free slot of BRKSIZE is created, detected in the second loop and used to store the slot.
malloc calls sbrk (see brk(2)) to get more memory from the system when there is no suitable space already available.
sbrk allocates more memory to a program (if possible), and returns a pointer to the start of the new memory area.
