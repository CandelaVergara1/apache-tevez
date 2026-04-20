#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

#define API_URL "https://api.worldbank.org/v2/en/country/AR/indicator/SI.POV.GINI?format=json&date=2011:2020&per_page=100"

extern int procesar_gini(float value);

struct buffer {
    char *data;
    size_t size;
};

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total = size * nmemb;
    struct buffer *buf = (struct buffer *)userp;
    char *temp = realloc(buf->data, buf->size + total + 1);
    if (!temp) return 0;
    buf->data = temp;
    memcpy(&(buf->data[buf->size]), contents, total);
    buf->size += total;
    buf->data[buf->size] = '\0';
    return total;
}

void fetch_api() {
    CURL *curl;
    CURLcode res;
    struct buffer buf = {NULL, 0};

    curl = curl_easy_init();
    if (!curl) {
        printf("Error al iniciar curl\n");
        return;
    }

    curl_easy_setopt(curl, CURLOPT_URL, API_URL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        printf("Error en request: %s\n", curl_easy_strerror(res));
        free(buf.data);
        return;
    }

    cJSON *json = cJSON_Parse(buf.data);
    cJSON *records = cJSON_GetArrayItem(json, 1);

    int count = cJSON_GetArraySize(records);
    for (int i = 0; i < count; i++) {
        cJSON *item = cJSON_GetArrayItem(records, i);
        cJSON *country = cJSON_GetObjectItem(item, "country");
        cJSON *name = cJSON_GetObjectItem(country, "value");
        cJSON *date = cJSON_GetObjectItem(item, "date");
        cJSON *value = cJSON_GetObjectItem(item, "value");

        if (!cJSON_IsNull(value)) {
            float gini = (float)value->valuedouble;
            int result = procesar_gini(gini);
            printf("%s - %s: %d\n", name->valuestring, date->valuestring, result);
        }
    }

    cJSON_Delete(json);
    free(buf.data);
}

int main(void) {
    fetch_api();
    return 0;
}