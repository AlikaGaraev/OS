#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



#define MAX_ITER 200
#define EPSILON 0.001



int main(void) {
    char *string = NULL;
    size_t size = 0;
    double line; 
    if((line = (double)getline(&string, &size, stdin)) != -1){
        printf("getline returned %f:\n", (double) line);
        return 0;
    }
    
    return 0;
}
