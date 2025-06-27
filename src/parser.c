#include "bearc.h"
#include <stdio.h>
#include <stdlib.h>

void parser_init(Parser *parser, char *input) {
  lexer_init(&parser->lexer, input);
  expr_pool_init(&parser->pool);
  parser->errors = 0;
}

typedef struct {
  unsigned char a;
  unsigned char b;
} Pair;

static Token parser_peak_token(Parser *parser) {
  Token token;
  Lexer lexer = parser->lexer;
  lexer_next(&lexer, &token);
  return token;
}
static Token parser_next_token(Parser *parser) {
  Token token;
  lexer_next(&parser->lexer, &token);
  return token;
}

static Pair infix_priority[TK_EOF + 1] = {
    [TK_MINUS] = {.a = 1, .b = 2},
    [TK_PLUS] = {.a = 1, .b = 2},
    [TK_SLASH] = {.a = 3, .b = 4},
    [TK_STAR] = {.a = 3, .b = 4},
};

static ExprRef parse_expr(Parser *parser, unsigned char min_b) {
  ExprRef lhs = 0;
  ExprRef rhs = 0;
  Pair pair = {0};
  Token token = parser_next_token(parser);

  switch (token.kind) {
  case TK_NUM:
    lhs = expr_literal(&parser->pool, token);
    break;
  case TK_LPAREN:
    lhs = parse_expr(parser, 0);
    lhs = expr_grouping(&parser->pool, token, lhs);
    token = parser_next_token(parser);
    if (token.kind != TK_RPAREN) {
      printf("Expected ')' token");
      abort();
    }
    break;
  default:
    printf("Expected literal or paren token");
    abort();
  }

  while (1) {
    token = parser_peak_token(parser);
    if (token.kind == TK_EOF) {
      break;
    }
    pair = infix_priority[token.kind];
    if (pair.a == 0 && pair.b == 0) {
      break;
    }
    if (pair.a < min_b) {
      break;
    }
    token = parser_next_token(parser);
    rhs = parse_expr(parser, pair.b);
    lhs = expr_binary(&parser->pool, token, lhs, rhs);
  }

  return lhs;
}

ExprRef parser_parse(Parser *parser) { return parse_expr(parser, 0); }
