#include "bearc.h"

static Expr *parse_literal(Parser *parser);
static Expr *parse_binary(Parser *parser);
static Expr *parse_expr(Parser *parser);

Parser *parser_new(char *input) {
  Parser *parser = (Parser *)calloc(1, sizeof(Parser));
  parser->curr = lex_start(input);
  return parser;
}

/* TODO: Remove */
Token next_token(Parser *parser) {
  Token t;
  return t;
}

static Expr *parse_binary(Parser *parser) {
  Expr *expr = parse_literal(parser);
  Token token = next_token(parser);
  switch (token.kind) {
  case TK_PLUS:
  case TK_MINUS:
  case TK_SLASH:
  case TK_SPLAT:
    next_token(parser);
    return expr_binary(token, expr, parse_literal(parser));
  default:
    exit(1);
  }
}

static Expr *parse_literal(Parser *parser) {
  switch (parser->curr.kind) {
  case TK_INT:
    return expr_literal(parser->curr);
  default:
    exit(1);
  }
}
