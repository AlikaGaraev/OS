#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define  _GNU_SOURCE


#define MAX_ITER 200
#define EPSILON 0.001



int main(int argc, char** argv) {
    char* string = NULL;
    double size = 0;
    char* line;
    line = getline(&string, &size, stdin);
    printf("getline returned %ld\n", line);
    return 0;
    }
