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
				
				if(strcmp(node->val.d[i].key, "title") == 0) {
					feedTitle = node->val.d[i].val->val.s;
					printf("Feed title: %s\n", feedTitle);
				}else if(strncmp(node->val.d[i].key, "item", 4) == 0){
					printf("New item:\n");
					int j;
					for(j = 0; node->val.d[i].val->val.d[j].val; ++j) {
						//~ The dictionary we're currently exploring
						be_dict currDict = node->val.d[i].val->val.d[j];					
						 
						if(strcmp(currDict.key, "title") == 0){
							printf("\ttitle: %s\n", currDict.val->val.s);
						}else if(strcmp(currDict.key, "description") == 0){
							printf("\tdescription: %s\n", currDict.val->val.s);
						}else if(strcmp(currDict.key, "link") == 0){
							printf("\tlink: %s\n", currDict.val->val.s);
						}else if(strcmp(currDict.key, "guid") == 0){
							printf("\tguid: %s\n", currDict.val->val.s);
						}else if(strcmp(currDict.key, "pubDate") == 0){
							printf("\tpubDate: %s\n", currDict.val->val.s);
						}
					 }
					 printf("\n");
				}
			}
		}
	}else{
		printf("Not yay\n");
	}
}