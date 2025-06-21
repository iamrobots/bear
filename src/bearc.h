#include <stdio.h>
#include <stdlib.h>

/* token.c */
typedef enum {
    TK_UNKNOWN,
    TK_EOF,
    TK_PLUS,
    TK_MINUS,
    TK_SLASH,
    TK_SPLAT,
    TK_LPAREN,
    TK_RPAREN,
    TK_SEMI_COLON,
    TK_DOT,
    TK_FLOAT,
    TK_INT
} TokenKind;

typedef struct Token Token;
struct Token {
    char* val;
    TokenKind kind;
    unsigned char len;
};


void print_token(Token *token);

/* lex.c */

Token lex_start(char *input);
Token lex_next(Token token);

/* expr.c */
typedef enum {EXPR_GROUP, EXPR_BINARY, EXPR_LITERAL} ExprKind;
typedef struct Expr Expr;
struct Expr {
    ExprKind kind;
    union {
        struct {Token op; Expr* expr;} Group;
        struct {Token op; Expr* lhs; Expr* rhs;} Binary;
        struct {
            Token tok;
            union {char* string; int isize; unsigned int usize;} value;
        } Literal;
    } value;
};


Expr *expr_group(Token op, Expr *expr);
Expr *expr_binary(Token op, Expr *lhs, Expr *rhs);
Expr *expr_literal(Token tok);

/* parser.c */

typedef struct Parser Parser;
struct Parser {
    Token curr;
};

Parser *new_parser(char *input);
Expr *parse(Parser *parser);
