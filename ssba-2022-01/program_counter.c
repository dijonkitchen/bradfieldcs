#include <stdio.h>

void * get_pc() {
    return __builtin_return_address(0);
}

int main() {
    printf("%p\n", get_pc());
    printf("%p\n", get_pc());
    printf("%p\n", get_pc());
    printf("%p\n", get_pc());
}