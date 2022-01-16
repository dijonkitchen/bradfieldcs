#include <stdio.h>
#include <string.h>

enum TokenType {
    WHITE_SPACE,

    NUMBER,
    DECIMAL_POINT,

    PLUS,
    NEGATION,
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
        struct Token *currentTokenPtr = &tokens[tokenCount];

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

        currentTokenPtr->lexeme = c;

        switch (c) {
            case '+':
                currentTokenPtr->type = PLUS;
                break;
            case '-':
                currentTokenPtr->type = NEGATION;
                break;
            case '(':
                currentTokenPtr->type = LEFT_PAREN;
                break;
            case ')':
                currentTokenPtr->type = RIGHT_PAREN;
                break;
            case '/':
                currentTokenPtr->type = DIVIDE;
                break;
            case '*':
                currentTokenPtr->type = MULTIPLY;
                break;
            case '^':
                currentTokenPtr->type = EXPONENT;
                break;
            case '.':
                currentTokenPtr->type = DECIMAL_POINT;
                break;
            case ' ':
                continue;
            default:
                currentTokenPtr->type = NUMBER;
                break;
        }

//        printf("type: %u, lexeme: %c\n", tokens[tokenCount].type, tokens[tokenCount].lexeme);

        tokenCount++;
    }

    for (int c = 0; c < tokenCount; c++) {
        printf("type: %u, lexeme: %c\n", tokens[c].type, tokens[c].lexeme);
    }

    return 0;
}