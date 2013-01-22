#include <minix/drivers.h>
#include <minix/vtreefs.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "bencode.h"
#define MAX_INODES 10


typedef struct
{
    char date[9];
    struct inode* node;
} dateNode;



dateNode dateNodeArray[MAX_INODES];
int lastDateNodeIndex;

/*
Basic program logic;
	-> get the title element of the RSS
	-> create a folder with that name
	-> loop though the nodes, and make a file for every node (the entire name does not fit)
	-> use the counter in cbdata as a file identifier (each file should have a unique number)
	-> use that number to get data from the node tree when asked for
	
	Errors;
	- unmounting is "difficult"
*/

/*
Reads a b-encoded file (/file.ben), and returns a node object representing the data.
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

static struct inode* lookupDateStringInDateArray(char* dateString, struct inode* parentDirectory) {
    printf("\tLookup date string in date array for string: %s \n",dateString);
    struct inode *dateDirectoryInode = NULL;
    int i;
    for (i =0; i < lastDateNodeIndex; i++) {
    	printf("\t\t Comparing [%s] with [%s]\n",dateNodeArray[i].date,dateString);
        if (strcmp(dateNodeArray[i].date,dateString) == 0) {
            // DateDirectory already exists, return it

    		printf("Found it for : %s \n",dateString);
            dateDirectoryInode = dateNodeArray[i].node;
            return dateDirectoryInode;
        }
    }
	printf("\tDidn't found it for : %s \n",dateString);
    struct inode_stat dir_stat;
    dir_stat.mode = S_IFDIR | 0444;
    dir_stat.uid = 0; //User id (owner)
    dir_stat.gid = 0; //Group id (owner)
    dir_stat.size = 0; //Size of the file
    dir_stat.dev = NO_DEV; //devise number
    
    //Create the inode for this date directory
    printf("\t*About to add a folder with name %s\n", dateString);
    dateDirectoryInode = add_inode(parentDirectory, dateString, NO_INDEX, &dir_stat, 0, (cbdata_t) 1);
    
    // Construct the dateNode for this inode.
    dateNode dateDirectoryNode;
    strcpy(dateDirectoryNode.date,dateString);
    dateDirectoryNode.node = dateDirectoryInode;
    
    //Add the date directory to the dateNodeArray;
    dateNodeArray[lastDateNodeIndex] = dateDirectoryNode;
    lastDateNodeIndex++;
    return dateDirectoryInode;
}

static void my_init_hook(void) {
	//Decode het ben file naar een be_node
	be_node *node = read_file_with_root_node();
	
	if(node) {
		if(node->type != BE_DICT) {
			printf("Invalid format.");
			exit(EXIT_FAILURE);
		} else {
			int i;
			int j;
       
		     struct inode *parentDirectory = NULL;

			printf("Starting with the feed...\n");
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
					}
				}
			}
			
			printf("Continuing to the files...\n");
			if(parentDirectory == NULL){
				parentDirectory = get_root_inode();
			}
			// We now have a parentDirectory (e.g. "UT Nieuws") In which we will put the date directories (01-2013, 02-2013, etc...)
			for (i = 0; node->val.d[i].val; ++i) {
				if(strncmp(node->val.d[i].key, "item", 4) == 0){
                    //Placeholders for the attributes of the item
                    char itemTitle [PNAME_MAX];
                    char dateString [9];

					for(j = 0; node->val.d[i].val->val.d[j].val; ++j) {
						//~ The dictionary we're currently exploring
						be_dict currDict = node->val.d[i].val->val.d[j];
                        
						if(strcmp(currDict.key, "title") == 0){
                            // Copy the title to the title placeholder, with a max length of PNAME_MAX
                            strncpy(itemTitle, currDict.val->val.s, (PNAME_MAX-1));
                            //~ Add a terminating character
                            itemTitle[PNAME_MAX -1] = '\0';
                            printf("Found a itemTitle: %s\n", itemTitle);
						} else if(strcmp(currDict.key, "pubDate") == 0){ //CHECK IF DATE IS RIGHT
							printf("Gotta parse that date: %s\n", currDict.val->val.s);
							// Copy the itemTitle to the itemTitle placeholder, with a max length of PNAME_MAX
                            strncpy(dateString, currDict.val->val.s + 8, 8); //van pos 8 tot pos 15
                            //~ Add a terminating character
                            dateString[8] = '\0';
						}
					}
                    
                    //~ Add the file;
                    struct inode_stat file_stat;
                    
                    file_stat.mode = S_IFREG | 0444;
                    file_stat.uid = 0; //User id (owner)
                    file_stat.gid = 0; //Group id (owner)
                    file_stat.size = 0; //Size of the file
                    file_stat.dev = NO_DEV; //devise number
                    
                    
                    struct inode *dateDirectory = NULL;
                        
                    //Lookup the month/year combination in the date array, or create a new one
                    dateDirectory = lookupDateStringInDateArray(dateString,parentDirectory);
                    
                    //Create the file (TODO: PLACE)
                    add_inode(dateDirectory, itemTitle, NO_INDEX, &file_stat, 0, (cbdata_t) i);
				}
			}
		}
	}else{
		printf("Node object was null\n");
	}
			
	be_free(node);
	
	printf("Finished creating filesystem.\n\n");
}
static int my_read_hook(struct inode *inode, off_t offset, char **ptr, size_t *len, cbdata_t cbdata) {
	/* This hook will be called every time a regular file is read. We use
	* it to dyanmically generate the contents of our file.
	*/
	static char data[1000];
	int j;
	be_node *node = read_file_with_root_node();
	char *name = NULL;
	char *date = NULL;
	char *descr = NULL;
	
	for(j = 0; node->val.d[(int) cbdata].val->val.d[j].val; ++j) {
		//~ The dictionary we're currently exploring
		be_dict currDict = node->val.d[(int)cbdata].val->val.d[j];

		if(strcmp(currDict.key, "title") == 0){
			name = currDict.val->val.s;
		}else if(strcmp(currDict.key, "pubDate") == 0) {
			date = currDict.val->val.s;
		}else if(strcmp(currDict.key, "description") == 0){
			descr = currDict.val->val.s;
		}
	}
	
	//~ Clean up resources
	be_free(node);
	
	//~ Create the file contents & put them in the *static* char* data
	//~ (notice the markdown style of making something a title...
	sprintf(data, "##%s\n#####%s\n%s\n\n\n", name, date, descr);
	
	printf("String contents; %s\n", data);

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