#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char **argv) {

	if(argc < 2) {
		printf("Usage: %s time_string\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	putenv("DATEMSK=datemasks.txt");

	struct tm *time;
	
	time = getdate(argv[1]);
	if(time == NULL) {
		printf("Sad panda. %d\n", getdate_err);
	}else{
		if(time->tm_mon+1 < 10) {
			printf("path: %d/0%d\n", time->tm_year+1900, time->tm_mon+1);
		}else{
			printf("path: %d/%d\n", time->tm_year+1900, time->tm_mon+1);
		}
	}

	return 0;
}