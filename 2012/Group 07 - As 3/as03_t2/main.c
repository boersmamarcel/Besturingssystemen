#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "bencode.h"

void printBen(char *filename) {
	struct stat fileStats;
	FILE *fp = fopen(filename, "r");
	char *contents = NULL;
	
	//~ Some checks, should exclude most exceptions
	if(!fp) {
		printf("File pointer failure at file %s.\n", filename);
		return;
	}
	
	if(stat(filename, &fileStats) != 0) {
		printf("Failure reading file %s.\n", filename);
		return;
	}
	
	//~ Allocate memory to read the file into & read it
	contents = malloc(fileStats.st_size);
	fread(contents, 1, fileStats.st_size, fp);
	
	be_node *node = be_decoden(contents, fileStats.st_size);
	if(node) {
		if(node->type != BE_DICT) {
			printf("Invalid format in file %s.\n", filename);
			return;
		}else{
			int i;
			
			char *feedTitle;
			
			//~ Loop through all top-level elements in this be_node. RSS has a depth of 1, so no recursion should be necessary
			for (i = 0; node->val.d[i].val; ++i) {
				
				if(strcmp(node->val.d[i].key, "title") == 0) {
					//~ The title of this feed, let's print that one as well
					feedTitle = node->val.d[i].val->val.s;
					printf("Feed title: %s\n", feedTitle);
				}else if(strncmp(node->val.d[i].key, "item", 4) == 0){
					//~ We found a new item! It will contain dictionary stuff
					printf("New item:\n");
					int j;
					for(j = 0; node->val.d[i].val->val.d[j].val; ++j) {
						//~ The dictionary we're currently exploring
						be_dict currDict = node->val.d[i].val->val.d[j];					
						 
						//~ Print the content of this dictionary
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
					 //~ Linebreak for layout...
					 printf("\n");
				}
			}
		}
	}else{
		printf("Not yay at file %s.\n", filename);
	}
}

int main(int argc, char **argv) {
	int i = 1;
	if(argc < 2) {
		printf("You should specify the file to be used...\nProgram terminated\n");
		exit(EXIT_SUCCESS);
	}
	
	for(; i < argc; i++) {
		printf("Converting file %s to custom format.\n", argv[i]);
		printBen(argv[i]);
		printf("\n");
	}
	
	return 0;
}