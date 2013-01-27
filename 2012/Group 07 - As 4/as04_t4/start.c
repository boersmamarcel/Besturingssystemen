#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	char *run = "./as4_t4 `./exploit 130 130`";
	system(run);
}
