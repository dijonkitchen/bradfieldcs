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
    // TODO: make into a pointer for arbitrary length
    char lexeme[111];
};

struct TokensAndCount {
    struct Token *pTokens;
    int count;
};

// AKA lexer or tokenizer
struct TokensAndCount *scanner(char *string) {
    int string_length = strlen(string);
    struct Token *pTokens = calloc(string_length, sizeof(struct Token));
    int tokenIndex = 0;

    for (int i = 0; i < string_length; i++) {
        char ch = string[i];
        struct Token *pCurrentToken = &pTokens[tokenIndex];

//        printf("i: %d, tokenCount: %d, ch: %c, &ch: %p \n", i, tokenIndex, ch, &ch);

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
                strncat(pPreviousToken->lexeme, &ch, 1);
                continue;
            }
        }

//        pCurrentToken->lexeme = strncat(pCurrentToken->lexeme, &ch, 1);
        strncpy(pCurrentToken->lexeme, &ch, 1);

        tokenIndex++;
    }

    struct TokensAndCount *pTokensAndCount = malloc(sizeof(char *));
    pTokensAndCount->pTokens = pTokens;
    pTokensAndCount->count = tokenIndex;

    return pTokensAndCount;
}


// Recursive descent parsing productions with a context-free EBNF grammar:
// <program_start> ::= <expression>
// <expression> ::= <terminal> | <grouping> | <expression> <operator> <expression>
// <terminal> ::= number
// <operator> ::= [+-*/^]
// <grouping> ::= LEFT_PAREN <expression> RIGHT_PAREN


// Examples:

// (1+2)*(3+4)
// program -> <expression>
// program -> <expression> <operator> <expression>
// program -> <grouping> <operator> <expression>
// program -> <grouping> <operator> <expression>
// program -> lparen <expression> rparen <operator> <expression>
// program -> lparen <expression> <operator> <expression> rparen <operator> <expression>
// program -> lparen <terminal> <operator> <expression> rparen <operator> <expression>
// program -> lparen number <operator> <expression> rparen <operator> <expression>
// program -> lparen number + <expression> rparen <operator> <expression>
// program -> lparen number + <terminal> rparen <operator> <expression>
// program -> lparen number + number rparen <operator> <expression>
// program -> lparen number + number rparen * <expression>
// program -> lparen number + number rparen * <grouping>
// program -> lparen number + number rparen * lparen <expression> rparen
// program -> lparen number + number rparen * lparen <expression> <operator> <expression> rparen
// program -> lparen number + number rparen * lparen <terminal> <operator> <expression> rparen
// program -> lparen number + number rparen * lparen number <operator> <expression> rparen
// program -> lparen number + number rparen * lparen number + <expression> rparen
// program -> lparen number + number rparen * lparen number + <terminal> rparen
// program -> lparen number + number rparen * lparen number + number rparen

// (1+ (2 + -3/0) ^ 4)
// program -> <expression>
// program -> <grouping>
// program -> lparen <expression> rparen
// program -> lparen <expression> <operator> <expression> rparen
// program -> lparen <terminal> <operator> <expression> rparen
// program -> lparen number <operator> <expression> rparen
// program -> lparen number + <expression> rparen
// program -> lparen number + <expression> <operator> <expression> rparen
// program -> lparen number + <grouping> <operator> <expression> rparen
// program -> lparen number + lparen <expression> <operator> <expression> rparen <operator> <expression> rparen
// program -> lparen number + lparen <terminal> <operator> <expression> rparen <operator> <expression> rparen
// program -> lparen number + lparen number <operator> <expression> rparen <operator> <expression> rparen
// program -> lparen number + lparen number + <expression> rparen <operator> <expression> rparen
// program -> lparen number + lparen number + <expression> <operator> <expression> rparen <operator> <expression> rparen
// program -> lparen number + lparen number + number <operator> <expression> rparen <operator> <expression> rparen
// program -> lparen number + lparen number + number / <expression> rparen <operator> <expression> rparen
// program -> lparen number + lparen number + number / number rparen <operator> <expression> rparen
// program -> lparen number + lparen number + number / number rparen ^ <expression> rparen
// program -> lparen number + lparen number + number / number rparen ^ <terminal> rparen
// program -> lparen number + lparen number + number / number rparen ^ number rparen


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
        printf("type: %u, lexeme: %s \n", pTokensAndCount->pTokens[c].type,
               pTokensAndCount->pTokens[c].lexeme);
//        printf("type: %u, lexeme: %s, &lexeme: %p \n", pTokensAndCount->pTokens[c].type,
//               pTokensAndCount->pTokens[c].lexeme, &pTokensAndCount->pTokens[c].lexeme);
    }

    return 0;
}