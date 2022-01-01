#include <stdio.h>
#include <time.h>

#define ITERATIONS 1000000000
#define GIGAHERTZ_OPERATIONS 1000000000

int main() {
    clock_t start = clock();

    for(int i = 0; i < ITERATIONS; i++) {
        int test = 1 / 1;
    }

    clock_t end = clock();

    float cpu_time_used = ((float) (end - start)) / CLOCKS_PER_SEC;
    float operations = ITERATIONS / cpu_time_used;

    printf("Clock speed approximately %.3f GHz\n", operations / GIGAHERTZ_OPERATIONS);

    return 0;
}