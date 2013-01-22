#include <minix/drivers.h>
#include <minix/vtreefs.h>
#include <sys/stat.h>
#include <time.h>
#include <assert.h>
#include "bencode.h"

#define MAX_INODES 10

typedef struct {
    struct inode *parent;
    struct inode *me;
    struct inode_stat stat;
    char *name;
} file;


file stack[MAX_INODES];
int counter;

static void emptyStack()
{
    int i;    
    for (i = 0; i < 1; i++){
    
        
        add_inode(stack[i].parent, stack[i].name, NO_INDEX, &stack[i].stat, 0,
                  (cbdata_t) 1);
    }
}

static struct inode* create_dir(char *name, struct inode *parent)
{
    
    printf("Create directory %s", name);
    struct inode_stat dir_stat;
    struct inode *dir;
    //Generate dir stats
    dir_stat.mode = S_IFDIR | 0444;
    dir_stat.uid = 0; //User id (owner)
    dir_stat.gid = 0; //Group id (owner)
    dir_stat.size = 0; //Size of the file
    dir_stat.dev = NO_DEV; //devise number
    
    file directory;
    
    if(strlen(name) < PNAME_MAX){
        
        if(parent == NULL){
            directory.parent = get_root_inode();
        }else{
            directory.parent = parent;
        }
        
        directory.me = dir;
        directory.stat = dir_stat;
        directory.name = name;

    }else{
        printf("File name to long\n");
    }
    
    
    
    stack[counter] = directory;
    counter++;
    
    return dir;
    
}

static void create_file(char *name, struct inode *parent)
{
    printf("Create file %s", name);
    
    struct inode_stat file_stat;
    struct inode *fNode;
    
    file_stat.mode = S_IFREG | 0444;
    file_stat.uid = 0; //User id (owner)
    file_stat.gid = 0; //Group id (owner)
    file_stat.size = 0; //Size of the file
    file_stat.dev = NO_DEV; //devise number
    
    file stackFile;
    
    if(strlen(name) < PNAME_MAX){
        if(parent == NULL){
            stackFile.parent = get_root_inode();
        }else{
            stackFile.parent = parent;
        }
        
        stackFile.me = fNode;
        stackFile.stat = file_stat;
        stackFile.name = name;
        
    }else{
        printf("File name to long\n");
    }
    
    
    
    stack[counter] = stackFile;
    counter++;
    
}

static be_node* read_file_with_root_node()
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
    
    //Decode het ben file naar een be_node
    be_node *node = be_decoden(contents, fileStats.st_size);
    
    
    //set counter to zero
    counter = 0;
    
    return node;
}



static void my_init_hook(void)
{

  //Decode het ben file naar een be_node
  be_node *node = read_file_with_root_node();
  if(node) {
    if(node->type != BE_DICT) {
     printf("Invalid format.");
     exit(EXIT_FAILURE);
   } else {
     int i;
       
       struct inode *parentDirectory;

      //Walk through the properties of the dict.
      for (i = 0; node->val.d[i].val; ++i) {
        printf("%s\n", node->val.d[i].key);
				if(strcmp(node->val.d[i].key, "title") == 0) { //We've landed on the title, use it
          printf("We hebben een title %s", node->val.d[i].val->val.s);
                    //title equals directory
                    
        parentDirectory = create_dir(node->val.d[i].val->val.s, NULL);
                    
        printf("Dir is niet null, yay");
      } else if(strncmp(node->val.d[i].key, "item", 4) == 0){
        
                    
                    int j;
                    char *title = NULL;
                    for(j = 0; node->val.d[i].val->val.d[j].val; ++j) {
						//~ The dictionary we're currently exploring
                      be_dict currDict = node->val.d[i].val->val.d[j];
                      
                      
                      if(strcmp(currDict.key, "title") == 0){
                            //filename
                        title = currDict.val->val.s;
                      }else if(strcmp(currDict.key, "description") == 0){
                            //content
                        
                       printf("description: %s\n", currDict.val->val.s);
                     }else if(strcmp(currDict.key, "link") == 0){
                            //content
                      
                       printf("link: %s\n", currDict.val->val.s);
                     }else if(strcmp(currDict.key, "guid") == 0){
                            //id
                      
                       printf("guid: %s\n", currDict.val->val.s);
                     }
                     else if(strcmp(currDict.key, "pubDate") == 0){
                            //content
                      
                       printf("pubDate: %s\n", currDict.val->val.s);
                     }
                   }
          
          if(title != NULL){
              
              //create_file(title, parentDirectory);
          }
              
                   
                 }
               }
             }
           }else{
            printf("Not yay\n");
          }
    
    emptyStack();
    
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

