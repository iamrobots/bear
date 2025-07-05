#ifndef BEARC_H
#define BEARC_H

typedef enum {
  TK_ERR,
  TK_LPAREN,
  TK_RPAREN,
  TK_SEMICOLON,
  TK_PLUS,
  TK_MINUS,
  TK_STAR,
  TK_SLASH,
  TK_NUM,
  TK_EOF
} TokenKind;

typedef struct Token Token;
struct Token {
  union {
    unsigned integer;
  } value;
  unsigned pos;
  TokenKind kind;
};

/* lex.c */

typedef struct Lexer Lexer;
struct Lexer {
  char *input;
  int pos;
};

void lexer_init(Lexer *lexer, char *input);
void lexer_next(Lexer *lexer, Token *out);

/* expr.c */

typedef unsigned int ExprRef;
typedef struct ExprPool ExprPool;
typedef struct Expr Expr;

typedef enum { EX_ERR, EX_LITERAL, EX_BINARY, EX_GROUPING } ExprKind;
struct Expr {
  union {
    union {
      unsigned usize;
    } literal;
    struct {
      ExprRef lhs;
      ExprRef rhs;
    } binary;
    ExprRef grouping;
  } value;
  Token token;
  ExprKind kind;
};

ExprRef expr_error(ExprPool *pool, Token token, ExprRef ref);
ExprRef expr_grouping(ExprPool *pool, Token token, ExprRef inner);
ExprRef expr_binary(ExprPool *pool, Token token, ExprRef lhs, ExprRef rhs);
ExprRef expr_literal(ExprPool *pool, Token token);

struct ExprPool {
  Expr *buff;
  unsigned len;
  unsigned cap;
};

void expr_pool_init(ExprPool *pool);
ExprRef expr_pool_push(ExprPool *pool, Expr expr);
Expr *expr_pool_get(ExprPool *pool, ExprRef ref);
void expr_pool_free(ExprPool *pool);
void pretty_print(ExprPool *pool, ExprRef ref);

/* parser.c */

typedef struct Parser Parser;
struct Parser {
  Lexer lexer;
  ExprPool pool;
  ExprRef errors;
};

void parser_init(Parser *parser, char *input);
ExprRef parser_parse(Parser *parser);

/* eval */

void eval(ExprPool *pool, ExprRef ref);

#endif
