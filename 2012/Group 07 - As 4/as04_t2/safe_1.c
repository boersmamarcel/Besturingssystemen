#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main( int ac, char** av ) {
        if( ac > 1 ) {
                char *buf = malloc(strlen(av[1]));
                strcpy( buf, av[1] );
        }
}
