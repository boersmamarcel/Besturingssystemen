#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "bencode.h"

int main(int argc, char **argv) {
	
	char *filename = "file.ben";
	
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
	
	be_node *node = be_decoden(contents, fileStats.st_size);
	if(node) {
		if(node->type != BE_DICT) {
			printf("Invalid format.");
			exit(EXIT_FAILURE);
		}else{
			int i;
			
			char *feedTitle;
			
			for (i = 0; node->val.d[i].val; ++i) {
				
				printf("%s\n", node->val.d[i].key);
				if(strcmp(node->val.d[i].key, "title") == 0) {
                    printf("title awesoem\n");
					feedTitle = node->val.d[i].key;
				}else if(strncmp(node->val.d[i].key, "item", 4) == 0){
                    
                    printf("blaat\n");
					int j;
					 for(j = 0; node->val.d[i].val->val.d[j].val; ++j) {
                         
                         if(strcmp(node->val.d[i].val->val.d[j].key, "language") == 0){
                             printf("language: %s\n", node->val.d[i].val->val.s);
                         }else if(strcmp(node->val.d[i].val->val.d[j].key, "link") == 0){
                             printf("link: %s\n", node->val.d[i].val->val.s);
                         }else if(strcmp(node->val.d[i].val->val.d[j].key, "guid") == 0){
                             printf("guid: %s\n", node->val.d[i].val->val.s);
                         }
                         
						// printf("%s: %s\n", node->val.d[i].val.d[j].key, node->val.d[i].val.d[j].val);
					 }
				}
			}
		}
	}else{
		printf("Not yay\n");
	}
}