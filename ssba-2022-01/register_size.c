#include <stdio.h>

int main () {
    printf("sizeOf(uintptr_t): %lu bits.\n", sizeof(uintptr_t) * 8);

    unsigned long n = 1L;

    for(int i = 0; i <= 128; i++) {
        n = n << 1;

        if (n == 0) {
            printf("Registers are %d-bits using bitshifting.", i + 1);
            return i;
        }
    }

    return 0;
}

