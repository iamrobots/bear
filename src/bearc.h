#include <stdio.h>

typedef struct Token Token;
typedef struct Lexer Lexer;
typedef struct Parser Parser;

// token.c
typedef enum {
    TK_EOF,
    TK_PLUS,
    TK_MINUS,
    TK_SLASH,
    TK_SPLAT,
    TK_LPAREN,
    TK_RPAREN,
    TK_SEMI_COLON,
    TK_NUM
} TokenKind;

struct Token {
    TokenKind kind;
    char* val;
};

void print_token(Token *token);

// lexer.c
struct Lexer {
    char *input;
    char *curr;
};

Lexer lexer_new(char *input);
Token next_token(Lexer *lexer);


// parser.c
struct Parser {
    Lexer lexer;
};

Parser parser_new(Lexer *lexer);
