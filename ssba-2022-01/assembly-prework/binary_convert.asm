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
    mov rbx, 0 ; string length
    mov rcx, 1 ; power of two

.loop:
    cmp rdi, 0
    jne .increaseLength
    jmp .math

.increaseLength:
    add rbx, 1
    add rdi, 1
    jmp .loop

.math:
    cmp rdi, 1
    jeq .addToSum
    jmp .advance

.addToSum:
    add rax, rcx
    jmp .math

.advance:
    add rbx, 1
    add rdi, 1
    mul rcx, 2
    jmp .math

.exit:
    ret