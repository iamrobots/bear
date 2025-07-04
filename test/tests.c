#include "../src/bearc.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

static int assert_expr_eq(ExprPool *lhs_pool, ExprRef lhs_ref,
                          ExprPool *rhs_pool, ExprRef rhs_ref) {
  Expr lhs = *expr_pool_get(lhs_pool, lhs_ref);
  Expr rhs = *expr_pool_get(rhs_pool, rhs_ref);

  ASSERT_EQ(lhs.kind, rhs.kind);
  ASSERT_EQ(lhs.token.kind, rhs.token.kind);
  ASSERT_EQ(lhs.token.pos, rhs.token.pos);

  if (lhs.kind != rhs.kind || lhs.token.kind != rhs.token.kind ||
      lhs.token.pos != rhs.token.pos ||
      lhs.token.value.integer != rhs.token.value.integer) {
    return 0;
  }

  switch (lhs.kind) {
  case EX_LITERAL:
    ASSERT_EQ(lhs.token.value.integer, rhs.token.value.integer);
    ASSERT_EQ(lhs.value.literal.integer, rhs.value.literal.integer);
    return lhs.value.literal.integer == rhs.value.literal.integer;
  case EX_BINARY:
    return assert_expr_eq(lhs_pool, lhs.value.binary.lhs, rhs_pool,
                          rhs.value.binary.lhs) &&
           assert_expr_eq(lhs_pool, lhs.value.binary.rhs, rhs_pool,
                          rhs.value.binary.rhs);
  case EX_ERR:
  case EX_GROUPING:
    return assert_expr_eq(lhs_pool, lhs.value.grouping, rhs_pool,
                          rhs.value.grouping);
  }

  return 1;
}

void lex_tests(void) {
  Lexer lexer;
  Token token;
  TokenKind expected[] = {TK_PLUS, TK_MINUS, TK_NUM, TK_LPAREN, TK_RPAREN,
                          TK_STAR, TK_SLASH, TK_EOF, TK_EOF};
  unsigned long i = 0;
  printf("start lex_tests\n");

  lexer_init(&lexer, "+ - 567 () * /");

  for (; i < (sizeof(expected) / sizeof(expected[0])); ++i) {
    lexer_next(&lexer, &token);
    ASSERT_EQ(expected[i], token.kind);
  }

  /* can lex numbers */
  lexer_init(&lexer, "1234567890");
  lexer_next(&lexer, &token);
  ASSERT_EQ(0, token.pos);
  ASSERT_EQ(TK_NUM, token.kind);
  ASSERT_EQ(1234567890, token.value.integer);
  printf("end lex_tests\n");
}

void parser_tests(void) {
  char *input = "(55 + 66) - 7";
  Token tokens[] = {{.pos = 0, .value.integer = 0, .kind = TK_LPAREN},
                    {.pos = 4, .value.integer = 0, .kind = TK_PLUS},
                    {.pos = 1, .value.integer = 55, .kind = TK_NUM},
                    {.pos = 6, .value.integer = 66, .kind = TK_NUM},
                    {.pos = 10, .value.integer = 0, .kind = TK_MINUS},
                    {.pos = 12, .value.integer = 7, .kind = TK_NUM}};
  Parser parser;
  ExprRef actual_ref;
  ExprRef expected_ref;
  ExprPool pool;
  expr_pool_init(&pool);
  expected_ref =
      expr_binary(&pool, tokens[4],
                  expr_grouping(&pool, tokens[0],
                                expr_binary(&pool, tokens[1],
                                            expr_literal(&pool, tokens[2]),
                                            expr_literal(&pool, tokens[3]))),
                  expr_literal(&pool, tokens[5]));

  parser_init(&parser, input);

  printf("start parser_tests\n");
  actual_ref = parser_parse(&parser);

  assert_expr_eq(&pool, expected_ref, &parser.pool, actual_ref);
  printf("end parser_tests\n");
}

void parser_tests2(void) {
  char *input = "55 + (66 - 7)";
  Token tokens[] = {{.pos = 0, .value.integer = 55, .kind = TK_NUM},
                    {.pos = 3, .value.integer = 0, .kind = TK_PLUS},
                    {.pos = 5, .value.integer = 0, .kind = TK_LPAREN},
                    {.pos = 6, .value.integer = 66, .kind = TK_NUM},
                    {.pos = 9, .value.integer = 0, .kind = TK_MINUS},
                    {.pos = 11, .value.integer = 7, .kind = TK_NUM}};
  ExprPool pool;
  Parser parser;
  ExprRef actual_ref;
  ExprRef expected_ref;
  expr_pool_init(&pool);
  expected_ref =
      expr_binary(&pool, tokens[1], expr_literal(&pool, tokens[0]),
                  expr_grouping(&pool, tokens[2],
                                expr_binary(&pool, tokens[4],
                                            expr_literal(&pool, tokens[3]),
                                            expr_literal(&pool, tokens[5]))));

  parser_init(&parser, input);

  printf("start parser_tests2\n");
  actual_ref = parser_parse(&parser);

  assert_expr_eq(&pool, expected_ref, &parser.pool, actual_ref);
  printf("end parser_tests2\n");
}

void parser_tests3(void) {
  char *input = "55 + 66 * 7";
  Token tokens[] = {{.pos = 0, .value.integer = 55, .kind = TK_NUM},
                    {.pos = 3, .value.integer = 0, .kind = TK_PLUS},
                    {.pos = 5, .value.integer = 66, .kind = TK_NUM},
                    {.pos = 8, .value.integer = 0, .kind = TK_STAR},
                    {.pos = 10, .value.integer = 7, .kind = TK_NUM}};
  ExprPool pool;
  Parser parser;
  ExprRef actual_ref;
  ExprRef expected_ref;
  expr_pool_init(&pool);
  expected_ref =
      expr_binary(&pool, tokens[1], expr_literal(&pool, tokens[0]),
                  expr_binary(&pool, tokens[3], expr_literal(&pool, tokens[2]),
                              expr_literal(&pool, tokens[4])));
  parser_init(&parser, input);

  printf("start parser_tests3\n");
  actual_ref = parser_parse(&parser);

  assert_expr_eq(&pool, expected_ref, &parser.pool, actual_ref);
  printf("end parser_tests3\n");
}

int main(void) {
  lex_tests();
  parser_tests();
  parser_tests2();
  parser_tests3();
  return 0;
}
