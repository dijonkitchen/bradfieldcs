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
    char lexeme[5];
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
        char ch = string[i];
        struct Token *pCurrentToken = &pTokens[tokenCount];

        printf("c: %c, i: %d, tokenCount: %d\n", ch, i, tokenCount);

//        if (i > 0) {
//            int previousType = pTokens[i - 1].type;
//            printf("prev type: %d", previousType);
//        }

//        else if (previousType < PLUS) {
//            tokens[i].lexeme = strcat(tokens[i].lexeme, (char[2]) {c, '\0'});
//            continue;
//        } else {
//            tokens[i].lexeme = (char[2]) {c, '\0'};
//        }
//        }

//        pCurrentToken->lexeme = strncat(pCurrentToken->lexeme, &ch, 1);
        strcpy(pCurrentToken->lexeme, &ch);

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

//        printf("type: %u, lexeme: %c\n", tokens[tokenCount].type, tokens[tokenCount].lexeme);

        tokenCount++;
    }

    struct TokensAndCount *pTokensAndCount = malloc(sizeof(char *));
    pTokensAndCount->pTokens = pTokens;
    pTokensAndCount->count = tokenCount;

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
        printf("type: %u, lexeme: %s\n", pTokensAndCount->pTokens[c].type, pTokensAndCount->pTokens[c].lexeme);
    }

    return 0;
}