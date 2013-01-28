#include <stdio.h>

int main( int ac, char** av )
{
    char *end;
    long value = strtol(av[1], &end, 10);
    if (end == number || *end != '\0' || errno == ERANGE || value > 100)
        return -1;
    int* something = (int*)malloc( (int)value );
}