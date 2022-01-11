section .text
    global binary_convert

binary_convert:
;#include <stdio.h>
;#include <string.h>
;#include <stdlib.h>
;
;int main(int argc, char* argv[]) {
;    char* string = argv[0];
;    int sum = 0;
;    int powerOfTwo = 1;
;
;    for (int i = strlen(string) - 1; i >= 0; i--) {
;        sum += (string[i] - '0') * powerOfTwo;
;        powerOfTwo = powerOfTwo * 2;
;    }
;
;    return sum;
;}
; intialize sum as zero
; get string length (until null terminal value)
; parse through string in reverse based on length, one char at a time
; calculate power of 2 for place
; multiply product by digit
; add to sum
; when no more digits,
; return sum
    mov rax, 0 ; sum
    mov rbx, 1 ; initial power of 2^0

	ret
