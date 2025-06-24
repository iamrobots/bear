#include "bearc.h"

void consume(Parser *parser);
int matches(Parser *parser, TokenKind kind);

static ExprRef parse_expression(Parser *parser);
static ExprRef parse_binary(Parser *parser);
static ExprRef parse_grouping(Parser *parser);
static ExprRef parse_primary(Parser *parser);

void parser_init(Parser *parser, char *input) {
  Token token = {0};
  lexer_init(&parser->lexer, input);
  expr_pool_init(&parser->pool);
  parser->errors = 0;
  parser->token = token;
}

ExprRef parser_parse(Parser *parser) {
  consume(parser);
  return parse_expression(parser);
}

static ExprRef parse_expression(Parser *parser) { return parse_binary(parser); }
static ExprRef parse_binary(Parser *parser) {
  ExprRef lhs = parse_grouping(parser);
  Token token = {0};
  switch (parser->token.kind) {
  case TK_PLUS:
  case TK_MINUS:
    token = parser->token;
    consume(parser);
    lhs = expr_binary(&parser->pool, token, lhs, parse_grouping(parser));
    break;
  default:
    break;
  }
  return lhs;
}

static ExprRef parse_grouping(Parser *parser) {
  ExprRef result;
  if (matches(parser, TK_LPAREN)) {
    Token token = parser->token;
    consume(parser);
    result = expr_grouping(&parser->pool, token, parse_expression(parser));
    if (matches(parser, TK_RPAREN)) {
      consume(parser);
    } else {
      parser->errors = expr_error(&parser->pool, parser->token, parser->errors);
    }
  } else {
    result = parse_primary(parser);
  }

  return result;
}

static ExprRef parse_primary(Parser *parser) {
  ExprRef result;
  switch (parser->token.kind) {
  case TK_NUM:
    result = expr_literal(&parser->pool, parser->token);
    break;
  default:
    parser->errors = expr_error(&parser->pool, parser->token, parser->errors);
    result = parser->errors;
  }
  consume(parser);
  return result;
}

void consume(Parser *parser) { lexer_next(&parser->lexer, &parser->token); }

int matches(Parser *parser, TokenKind kind) {
  return parser->token.kind == kind;
}
