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
    int tokenCount = 0;

    for (int i = 0; i < string_length; i++) {
        char c = string[i];
        printf("c: %c, i: %d, tokenCount: %d\n", c, i, tokenCount);

//        if (i != 0) {
//            int previousType = tokens[i - 1].type;
//            printf("prev type: %d", previousType);

//        else if (previousType < PLUS) {
//            tokens[i].lexeme = strcat(tokens[i].lexeme, (char[2]) {c, '\0'});
//            continue;
//        } else {
//            tokens[i].lexeme = (char[2]) {c, '\0'};
//        }
//        }

        struct Token *currentToken = &tokens[tokenCount];

        currentToken->lexeme = c;

        switch (c) {
            case '+':
                currentToken->type = PLUS;
                break;
            case '-':
                currentToken->type = MINUS;
                break;
            case '(':
                currentToken->type = LEFT_PAREN;
                break;
            case ')':
                currentToken->type = RIGHT_PAREN;
                break;
            case '/':
                currentToken->type = DIVIDE;
                break;
            case '*':
                currentToken->type = MULTIPLY;
                break;
            case '^':
                currentToken->type = EXPONENT;
                break;
            case '.':
                currentToken->type = DECIMAL_POINT;
                break;
            case ' ':
                continue;
            default:
                currentToken->type = NUMBER;
                break;
        }

//        printf("type: %u, lexeme: %c\n", tokens[tokenCount].type, tokens[tokenCount].lexeme);

        tokenCount++;
    }

    for (int i = 0; i < tokenCount; i++) {
        printf("type: %u, lexeme: %c\n", tokens[i].type, tokens[i].lexeme);
    }

    return 0;
}