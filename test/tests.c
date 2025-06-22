#include "../src/bearc.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

static int assert_expr(ExprPool *lhs_pool, ExprRef lhs_ref, ExprPool *rhs_pool,
                       ExprRef rhs_ref) {
  Expr lhs = *expr_pool_get(lhs_pool, lhs_ref);
  Expr rhs = *expr_pool_get(rhs_pool, rhs_ref);

  ASSERT_EQ(lhs.kind, rhs.kind);
  ASSERT_EQ(lhs.token.kind, rhs.token.kind);
  ASSERT_EQ(lhs.token.pos, rhs.token.pos);

  if (lhs.kind != rhs.kind || lhs.token.kind != rhs.token.kind ||
      lhs.token.pos != rhs.token.pos || lhs.token.num != rhs.token.num) {
    return 0;
  }

  switch (lhs.kind) {
  case EX_LITERAL:
    ASSERT_EQ(lhs.token.num, rhs.token.num);
    ASSERT_EQ(lhs.number, rhs.number);
    return lhs.number == rhs.number;
  case EX_BINARY:
    return assert_expr(lhs_pool, lhs.binary.lhs, rhs_pool, rhs.binary.lhs) &&
           assert_expr(lhs_pool, lhs.binary.rhs, rhs_pool, rhs.binary.rhs);
  case EX_ERR:
  case EX_GROUPING:
    return assert_expr(lhs_pool, lhs.inner, rhs_pool, rhs.inner);
  }

  return 1;
}

void lex_tests(void) {
  printf("start lex_tests\n");
  Lexer lexer;
  Token token;
  TokenKind expected[] = {TK_PLUS,   TK_MINUS, TK_NUM, TK_LPAREN,
                          TK_RPAREN, TK_EOF,   TK_EOF};
  lexer_init(&lexer, "+ - 567 ()");
  for (unsigned long i = 0; i < (sizeof(expected) / sizeof(expected[0])); ++i) {
    lexer_next(&lexer, &token);
    ASSERT_EQ(expected[i], token.kind);
  }

  /* can lex numbers */
  lexer_init(&lexer, "1234567890");
  lexer_next(&lexer, &token);
  ASSERT_EQ(0, token.pos);
  ASSERT_EQ(TK_NUM, token.kind);
  ASSERT_EQ(1234567890, token.num);
  printf("end lex_tests\n");
}

void parser_tests(void) {
  printf("start parser_tests\n");
  Token tokens[] = {{.pos = 0, .num = 0, .kind = TK_LPAREN},
                    {.pos = 4, .num = 0, .kind = TK_PLUS},
                    {.pos = 1, .num = 55, .kind = TK_NUM},
                    {.pos = 6, .num = 66, .kind = TK_NUM},
                    {.pos = 10, .num = 0, .kind = TK_MINUS},
                    {.pos = 12, .num = 7, .kind = TK_NUM}};
  ExprPool pool;
  Parser parser;
  ExprRef actual_ref;
  ExprRef expected_ref;
  expr_pool_init(&pool);
  expected_ref =
      expr_binary(&pool, tokens[4],
                  expr_grouping(&pool, tokens[0],
                                expr_binary(&pool, tokens[1],
                                            expr_literal(&pool, tokens[2]),
                                            expr_literal(&pool, tokens[3]))),
                  expr_literal(&pool, tokens[5]));

  parser_init(&parser, "(55 + 66) - 7");
  actual_ref = parser_parse(&parser);
  assert_expr(&pool, expected_ref, &parser.pool, actual_ref);
  printf("end parser_tests\n");
}

void parser_tests2(void) {
  printf("start parser_tests2\n");
  Token tokens[] = {{.pos = 0, .num = 55, .kind = TK_NUM},
                    {.pos = 3, .num = 0, .kind = TK_PLUS},
                    {.pos = 5, .num = 0, .kind = TK_LPAREN},
                    {.pos = 6, .num = 66, .kind = TK_NUM},
                    {.pos = 9, .num = 0, .kind = TK_MINUS},
                    {.pos = 11, .num = 7, .kind = TK_NUM}};
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

  parser_init(&parser, "55 + (66 - 7)");
  actual_ref = parser_parse(&parser);
  assert_expr(&pool, expected_ref, &parser.pool, actual_ref);
  printf("end parser_tests2\n");
}

int main(void) {
  lex_tests();
  parser_tests();
  parser_tests2();
  return 0;
}
