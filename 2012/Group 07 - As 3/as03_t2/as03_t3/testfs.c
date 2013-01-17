#include <minix/drivers.h>
#include <minix/vtreefs.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "bencode.h"
#define MAX_INODES 10

/*
Basic program logic;
	-> get the title element of the RSS
	-> create a folder with that name
	-> loop though the nodes, and make a file for every node (the entire name does not fit)
	-> use the counter in cbdata as a file identifier (each file should have a unique number)
	-> use that number to get data from the node tree when asked for
	
	Errors;
	- data is transfered in one big blob when asked for
	- unmounting is "difficult"
*/

static be_node* read_file_with_root_node() {
	// Lees het benfile in
	char *filename = "/file.ben";

	struct stat fileStats;
	FILE *fp = fopen(filename, "r");
	char *contents = NULL;

	if(!fp) {
		printf("File pointer failure.");
		exit(EXIT_FAILURE);
	}

	if(stat(filename, &fileStats) != 0) {
		printf("Failure.");
		exit(EXIT_FAILURE);
	}

	contents = malloc(fileStats.st_size);
	fread(contents, 1, fileStats.st_size, fp);

	//Decode het ben file naar een be_node
	be_node *node = be_decoden(contents, fileStats.st_size);

	return node;
}

static void my_init_hook(void) {
	//Decode het ben file naar een be_node
	be_node *node = read_file_with_root_node();
	
	if(node) {
		if(node->type != BE_DICT) {
			printf("Invalid format.");
			exit(EXIT_FAILURE);
		} else {
			printf("wazoookie");
			int i;
			int j;
       
		       struct inode *parentDirectory;

			//Walk through the properties of the dict to find the title of the rss feed
			for (i = 0; node->val.d[i].val; ++i) {
				if(strcmp(node->val.d[i].key, "title") == 0) { //We've landed on the title, use it
					char *title = node->val.d[i].val->val.s;
					printf("We hebben een title %s\n", title);
					
					if(strlen(title) < PNAME_MAX){
						//~ Create the directory, and adjust parentDirectory pointer;
						struct inode_stat dir_stat;
						dir_stat.mode = S_IFDIR | 0444;
						dir_stat.uid = 0; //User id (owner)
						dir_stat.gid = 0; //Group id (owner)
						dir_stat.size = 0; //Size of the file
						dir_stat.dev = NO_DEV; //devise number
						
						printf("About to add a folder with name %s\n", title);
						parentDirectory = add_inode(get_root_inode(), title, NO_INDEX, &dir_stat, 0, (cbdata_t) 1);
					}
					else {
						printf("Error; title name to long (%s)", title);
						exit(EXIT_FAILURE);
					}
				}
			}
			
			printf("Continuing to the files...\n");
			if(parentDirectory == NULL){
				parentDirectory = get_root_inode();
			}
			
			for (i = 0; node->val.d[i].val; ++i) {
				if(strncmp(node->val.d[i].key, "item", 4) == 0){
					for(j = 0; node->val.d[i].val->val.d[j].val; ++j) {
						//~ The dictionary we're currently exploring
						be_dict currDict = node->val.d[i].val->val.d[j];

						if(strcmp(currDict.key, "title") == 0){
							//~ Add the file;
							struct inode_stat file_stat;
							
							file_stat.mode = S_IFREG | 0444;
							file_stat.uid = 0; //User id (owner)
							file_stat.gid = 0; //Group id (owner)
							file_stat.size = 0; //Size of the file
							file_stat.dev = NO_DEV; //devise number
							
							char buffer [PNAME_MAX];
							strncpy (buffer, currDict.val->val.s, (PNAME_MAX-1) );
							buffer[PNAME_MAX-1] = '\0';
						    
							add_inode(parentDirectory, buffer, NO_INDEX, &file_stat, 0, (cbdata_t) i);
						}
					}
				}
			}
		}
	}else{
		printf("Not yay\n");
	}
}

static int my_read_hook(struct inode *inode, off_t offset, char **ptr, size_t *len, cbdata_t cbdata) {
	/* This hook will be called every time a regular file is read. We use
	* it to dyanmically generate the contents of our file.
	*/
	static char data[2000];
	char *str;
	int j;

	/* We have only a single file. With more files, cbdata may help
	* distinguishing between them.
	*/
	//assert((int) cbdata == 1);

	be_node *node = read_file_with_root_node();
	char *name;
	char *descr;
	for(j = 0; node->val.d[(int) cbdata].val->val.d[j].val; ++j) {
		//~ The dictionary we're currently exploring
		be_dict currDict = node->val.d[(int)cbdata].val->val.d[j];

		if(strcmp(currDict.key, "title") == 0){
			name = currDict.val->val.s;
		}else if(strcmp(currDict.key, "description") == 0){
			descr = currDict.val->val.s;
		}
	}
	
	sprintf(str, "%s\n%s", name, descr);
	
	printf("String contents; %s", str);

	strcpy(data, str);

	/* If the offset is beyond the end of the string, return EOF. */
	if (offset > strlen(data)) {
		*len = 0;
		return OK;
	}

	/* Otherwise, return a pointer into 'data'. If necessary, bound the
	* returned length to the length of the rest of the string. Note that
	* 'data' has to be static, because it will be used after this function
	* returns.
	*/
	*ptr = data + offset;

	if (*len > strlen(data) - offset)
		*len = strlen(data) - offset;

	return OK;
}

/* The table with callback hooks. */
struct fs_hooks my_hooks = {
        my_init_hook,
        NULL, /* cleanup_hook */
        NULL, /* lookup_hook */
        NULL, /* getdents_hook */
        my_read_hook,
        NULL, /* rdlink_hook */
        NULL  /* message_hook */
};