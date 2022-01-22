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
    char lexeme[16];
};

struct TokensAndCount {
    struct Token *pTokens;
    int count;
};

struct TokensAndCount *scanner(char *string) {
    int string_length = strlen(string);
    struct Token *pTokens = calloc(string_length, sizeof(struct Token));
    int tokenIndex = 0;

    for (int i = 0; i < string_length; i++) {
        char ch = string[i];
        struct Token *pCurrentToken = &pTokens[tokenIndex];

        printf("i: %d, tokenCount: %d, ch: %c, &ch: %p \n", i, tokenIndex, ch, &ch);

        switch (ch) {
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

        if (tokenIndex > 0) {
            struct Token *pPreviousToken = &pTokens[tokenIndex - 1];
//            printf("prev type: %d\n", previousType);
//            printf("less than plus?: %d\n", pCurrentToken->type < PLUS);
            if (pPreviousToken->type < PLUS && pCurrentToken->type < PLUS) {
                strcat(pPreviousToken->lexeme, &ch);
                continue;
            }
        }

//        pCurrentToken->lexeme = strncat(pCurrentToken->lexeme, &ch, 1);
        strcpy(pCurrentToken->lexeme, &ch);

        tokenIndex++;
    }

    struct TokensAndCount *pTokensAndCount = malloc(sizeof(char *));
    pTokensAndCount->pTokens = pTokens;
    pTokensAndCount->count = tokenIndex;

    return pTokensAndCount;
}

// Examples:

// (1+2)*(3+4)
// grouping -> lparen expression rparen
//          -> lparen atom + atom rparen
//          -> lparen atom + atom rparen * grouping
//          -> lparen atom + atom rparen * lparen expression rparen
//          -> lparen atom + atom rparen * lparen atom + atom rparen

// (1+ (2 + -3/0) ^ 4)
// grouping -> lparen expression rparen
//          -> lparen atom + expression rparen
//          -> lparen atom + grouping rparen
//          -> lparen atom + lparen expression rparen rparen
//          -> lparen atom + lparen atom + expression rparen rparen
//          -> lparen atom + lparen atom + binaryOperation rparen rparen
//          -> lparen atom + lparen atom + atom / atom rparen rparen
//          -> lparen atom + lparen atom + atom / atom rparen ^ atom rparen



// Recursive descent parsing with a context-free EBNF grammar:
// Non-terminal to terminal productions:
// <program> ::= <expression>
// <expression> ::= <term> ([+-*/^] <term>)*
// <term> ::= <grouping> | number
// <grouping> ::= LEFT_PAREN <expression> RIGHT_PAREN


//struct AbstractSyntaxTreeNode {
//    enum operator
//    struct AbstractSyntaxTreeNode *left;
//    struct AbstractSyntaxTreeNode *right;
//};
//
//enum Terminals {
//    NUMBER,
//    PLUS,
//    NEGATION,
//    DIVIDE,
//    MULTIPLY,
//    EXPONENT,
//};
//
//struct ContextFreeGrammar {
//
//};
//
//struct AbstractSyntaxTreeNode *parser(struct TokensAndCount *tokensAndCountPtr) {
//
//}

int main(int argc, char *argv[]) {
    char *string = argv[1];

    struct TokensAndCount *pTokensAndCount = scanner(string);

    for (int c = 0; c < pTokensAndCount->count; c++) {
        printf("type: %u, lexeme: %s, &lexeme: %p \n", pTokensAndCount->pTokens[c].type,
               pTokensAndCount->pTokens[c].lexeme, &pTokensAndCount->pTokens[c].lexeme);
    }

    return 0;
}