#include "bearc.h"

int main(void) {
  char *input = "(567 + 3) * (4 / 6);";
  printf("input = %s\n", input);
  Lexer lexer = lexer_new(input);
  Token t = next_token(&lexer);
  while (t.kind != TK_EOF) {
    print_token(&t);
    printf(" ");
    t = next_token(&lexer);
  }
  printf("\n");
}
