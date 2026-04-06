#include <stdio.h>
#include <stdlib.h>

int * num_conversor(float *gini_values, int quantity) {

    int *results = malloc(quantity * sizeof(int));

    if (results == NULL) {
        printf("Please, insert float type numbers on program execution.\n");
        return NULL;
    }


    for (int i = 0; i < quantity; i++) {
        results[i] = (int)(gini_values[i] + 1);
    }

    return results;
}

