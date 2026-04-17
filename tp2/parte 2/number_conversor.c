#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include "cJSON.h"


#define API_URL "https://api.worldbank.org/v2/en/country/AR/indicator/SI.POV.GINI?format=json&date=2011:2020&per_page=100"

extern int procesar_gini(float value);
int *fetch_api();

struct buffer {
    char *data;
    size_t size;
};

int main(void) {
    // Le delega el laburo a ASM
    float value = 2.3f;
    int result = procesar_gini(value);
    printf("%d\n", result);
    return 0;
}

int *fetch_api() {
    

    return NULL;
}

