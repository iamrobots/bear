#ifndef BEARC_H
#define BEARC_H

typedef enum {
    TK_ERR,
    TK_EOF,
    TK_LPAREN,
    TK_RPAREN,
    TK_PLUS,
    TK_MINUS,
    TK_NUM,
} TokenKind;

typedef struct Token Token;
struct Token {
    unsigned int pos;
    unsigned int size;
    union {
        int num;
    };
    TokenKind kind;
};

typedef struct Lexer Lexer;
struct Lexer {
    char* input;
    int pos;
};

void lexer_init(Lexer *lexer, char* input);
void lexer_next(Lexer *lexer, Token *out);

#endif
