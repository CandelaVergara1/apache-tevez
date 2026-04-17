#include <stdio.h>

extern int procesar_gini(float value);

int main(int argc, char *argv[]) {
    // Le delega el laburo a ASM
    float value = 4.3f;
    int result = procesar_gini(value);
    printf("%d\n", result);
    return 0;
}

