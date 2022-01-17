#include <stdio.h>
#include <stdlib.h>
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

struct TokensAndCount {
    struct Token *pTokens;
    int count;
};

struct TokensAndCount *scanner(char *string) {
    int string_length = strlen(string);
    struct Token *pTokens = calloc(string_length, sizeof(struct Token));
    int tokenCount = 0;

    for (int i = 0; i < string_length; i++) {
        char c = string[i];
        printf("c: %c, i: %d, tokenCount: %d\n", c, i, tokenCount);
        struct Token *pCurrentToken = &pTokens[tokenCount];

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

        pCurrentToken->lexeme = c;

        switch (c) {
            case '+':
                pCurrentToken->type = PLUS;
                break;
            case '-':
                pCurrentToken->type = NEGATION;
                break;
            case '(':
                pCurrentToken->type = LEFT_PAREN;
                break;
            case ')':
                pCurrentToken->type = RIGHT_PAREN;
                break;
            case '/':
                pCurrentToken->type = DIVIDE;
                break;
            case '*':
                pCurrentToken->type = MULTIPLY;
                break;
            case '^':
                pCurrentToken->type = EXPONENT;
                break;
            case '.':
                pCurrentToken->type = DECIMAL_POINT;
                break;
            case ' ':
                continue;
            default:
                pCurrentToken->type = NUMBER;
                break;
        }

//        printf("type: %u, lexeme: %c\n", tokens[tokenCount].type, tokens[tokenCount].lexeme);

        tokenCount++;
    }

    struct TokensAndCount *pTokensAndCount = malloc(sizeof(struct Token));
    pTokensAndCount->pTokens = pTokens;
    pTokensAndCount->count = tokenCount;

    return pTokensAndCount;
}

int main(int argc, char *argv[]) {
    char *string = argv[1];

    struct TokensAndCount *pTokensAndCount = scanner(string);

    for (int c = 0; c < pTokensAndCount->count; c++) {
        printf("type: %u, lexeme: %c\n", pTokensAndCount->pTokens[c].type, pTokensAndCount->pTokens[c].lexeme);
    }

    return 0;
}