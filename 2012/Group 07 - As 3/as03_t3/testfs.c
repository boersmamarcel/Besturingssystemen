#include <minix/drivers.h>
#include <minix/vtreefs.h>
#include <sys/stat.h>
#include <time.h>
#include <assert.h>
#include "bencode.h"

static void my_init_hook(void)
{
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
	// File.ben staat nu in contents

  //Decode het ben file naar een be_node
	be_node *node = be_decoden(contents, fileStats.st_size);
	if(node) {
		if(node->type != BE_DICT) {
			printf("Invalid format.");
			exit(EXIT_FAILURE);
		} else {
			int i;
      struct inode_stat dir_stat;
      struct inode *dir;
      //Generate dir stats
      dir_stat.mode = S_IFDIR | 0444;
      dir_stat.uid = 0; //User id (owner)
      dir_stat.gid = 0; //Group id (owner)
      dir_stat.size = 0; //Size of the file
      dir_stat.dev = NO_DEV; //devise number


      //Walk through the properties of the dict.
			for (i = 0; node->val.d[i].val; ++i) {
				printf("%s\n", node->val.d[i].key);
				if(strcmp(node->val.d[i].key, "title") == 0) { //We've landed on the title, use it
                    printf("We hebben een title");
                    //title equals directory
                    dir = add_inode(get_root_inode(), node->val.d[i].val->val.s, NO_INDEX, &dir_stat, 0,
                                        (cbdata_t) 1);
                    assert(dir != NULL);
                    printf("Dir is niet null, yay");
				} else if(strncmp(node->val.d[i].key, "item", 4) == 0){
                    
     //                struct inode_stat file_stat;
     //                struct inode *file;
                    
     //                file_stat.mode = S_IFREG | 0444;
     //                file_stat.uid = 0; //User id (owner)
     //                file_stat.gid = 0; //Group id (owner)
     //                file_stat.size = 0; //Size of the file
     //                file_stat.dev = NO_DEV; //devise number
                    
                    
					// int j;
     //                char *title;
					// for(j = 0; node->val.d[i].val->val.d[j].val; ++j) {
					// 	//~ The dictionary we're currently exploring
					// 	be_dict currDict = node->val.d[i].val->val.d[j];
                        
                        
					// 	if(strcmp(currDict.key, "title") == 0){
     //                        //filename
     //                        title = currDict.val->val.s;
					// 	}else if(strcmp(currDict.key, "description") == 0){
     //                        //content
                            
					// 		printf("description: %s\n", currDict.val->val.s);
					// 	}else if(strcmp(currDict.key, "link") == 0){
     //                        //content
                            
					// 		printf("link: %s\n", currDict.val->val.s);
					// 	}else if(strcmp(currDict.key, "guid") == 0){
     //                        //id
                            
					// 		printf("guid: %s\n", currDict.val->val.s);
					// 	}
					// 	else if(strcmp(currDict.key, "pubDate") == 0){
     //                        //content
                            
					// 		printf("pubDate: %s\n", currDict.val->val.s);
					// 	}
     //                }
                    
                    
                    // file = add_inode(dir, title, NO_INDEX, &file_stat, 0, (cbdata_t) 1);
				}
			}
		}
	}else{
		printf("Not yay\n");
	}
    
}

static int my_read_hook(struct inode *inode, off_t offset, char **ptr,
        size_t *len, cbdata_t cbdata)
{
        /* This hook will be called every time a regular file is read. We use
         * it to dyanmically generate the contents of our file.
         */
        static char data[32];
        const char *str;
        time_t now;

        /* We have only a single file. With more files, cbdata may help
         * distinguishing between them.
         */
        assert((int) cbdata == 1);

        /* Generate the contents of the file into the 'data' buffer. We could
         * use the return value of ctime() directly, but that would make for a
         * lousy example.
         */
        time(&now);

        str = "Hello world! Group 07 was here.";

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

