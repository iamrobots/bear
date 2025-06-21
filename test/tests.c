#include "../src/bearc.h"
#include "utils.h"

int main(void) {
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
  return 0;
}
