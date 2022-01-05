#include <stdio.h>
#include <time.h>
#include <stdbool.h>

int main() {
    int arr_size = 1000000;
    int arr[arr_size];

    for (int i = 0; i < arr_size; i++) {
        arr[i] = i;
    }

    clock_t seq_start = clock();

    int latest;
    for (int i = 0; i < arr_size; i++) {
        latest = arr[i];
    }

    clock_t seq_end = clock();

    bool add = true;
    int index = arr_size / 2;

    clock_t non_seq_start = clock();

    for (int i = 0; i < arr_size; i++) {
        if (add) {
            index = index + i;
            add = false;
        } else {
            index = index - i;
            add = true;
        }

        latest = arr[index];
    }

    clock_t non_seq_end = clock();

    printf("Sequential/cached time: %lu\n", seq_end - seq_start);
    printf("Non-sequential/non-cached time: %lu\n", non_seq_end - non_seq_start);
}