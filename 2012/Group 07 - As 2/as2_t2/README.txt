malloc calls sbrk (see brk(2)) to get more memory from the system when there is no suitable space already free.


sbrk allocates more memory to a program (if possible), and returns a pointer to the start of the new memory area


Steps to reproduce:

man malloc
man brk 2
find / -name malloc.c (according to the hints, malloc is implemented in a file called malloc.c )