#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define  _GNU_SOURCE


#define MAX_ITER 200
#define EPSILON 0.001



int main(void) {
    char* string = NULL;
    size_t size = 0;
    ssize_t line; 
    if(line = getline(&string, &size, stdin) == -1){
        exit(1);
    }
    printf("getline returned %s\n", line);
    return 0;
    }
