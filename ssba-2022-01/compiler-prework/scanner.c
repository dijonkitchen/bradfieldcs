#include <stdio.h>
#include <string.h>

enum TokenType {
    WHITE_SPACE,

    NUMBER,
    DECIMAL_POINT,

    PLUS,
    MINUS,
    LEFT_PAREN,
    RIGHT_PAREN,
    DIVIDE,
    MULTIPLY,
    EXPONENT,
};

struct Token {
    enum TokenType type;
    char lexeme;
//    char *lexeme;
};

int main(int argc, char *argv[]) {
    char *string = argv[1];
    int string_length = strlen(string);
    struct Token tokens[string_length];

    for (int i = 0; i < string_length; i++) {
        char c = string[i];
//        int previousType = tokens[i - 1].type;
//
//        if (previousType == WHITE_SPACE) {
//            continue;
//        } else if (previousType < PLUS) {
//            tokens[i].lexeme = strcat(tokens[i].lexeme, (char[2]) {c, '\0'});
//            continue;
//        } else {
//        tokens[i].lexeme = (char[2]) {c, '\0'};
//        }

        tokens[i].lexeme = c;

        switch (c) {
            case '+':
                tokens[i].type = PLUS;
                break;
            case '-':
                tokens[i].type = MINUS;
                break;
            case '(':
                tokens[i].type = LEFT_PAREN;
                break;
            case ')':
                tokens[i].type = RIGHT_PAREN;
                break;
            case '/':
                tokens[i].type = DIVIDE;
                break;
            case '*':
                tokens[i].type = MULTIPLY;
                break;
            case '^':
                tokens[i].type = EXPONENT;
                break;
            case '.':
                tokens[i].type = DECIMAL_POINT;
                break;
            case ' ':
                tokens[i].type = WHITE_SPACE;
                break;
            default:
                tokens[i].type = NUMBER;
                break;
        }
    }

    for (int i = 0; i < strlen(string); i++) {
        printf("lexeme: %c\n", tokens[i].lexeme);
        printf("type: %u\n", tokens[i].type);
    }

    return 0;
}