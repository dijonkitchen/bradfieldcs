#include <stdio.h>

int main() {
    char letter = 'a';

    for(int i = 0; i < 26; i++) {
        printf("%c", letter);
        letter++;
    }

    return 0;
}
