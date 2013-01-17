#include <stdio.h>
#include <minix/drivers.h>
#include <minix/vtreefs.h>
#include <sys/stat.h>
#include <time.h>
#include <assert.h>
#include "testfs.h"
#include "bencode.h"



int main(int argv, char* argc[])
{
    
    printf("blaat\n");
    
    struct inode_stat root_stat;
    
    /* Fill in the details to be used for the root inode. It will be a
     * directory, readable and searchable by anyone, and owned by root.
     */
    root_stat.mode = S_IFDIR | 0555;
    root_stat.uid = 0;
    root_stat.gid = 0;
    root_stat.size = 0;
    root_stat.dev = NO_DEV;
    
    /* Now start VTreeFS. Preallocate 10 inodes, which is more than we'll
     * need for this example. No indexed entries are used.
     */
    start_vtreefs(&my_hooks, 10, &root_stat, 0);
    
    /* The call above never returns. This just keeps the compiler happy. */
    return 0;

    
}