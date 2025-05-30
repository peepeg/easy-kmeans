#include <stdio.h>
#include <stdlib.h>


int main () {
    int *bruh = (int*) malloc(5 * sizeof(int));
    
    for (int i = 0; i < 5; i++)
        printf("%d ", bruh[i]);
    return 0;
}