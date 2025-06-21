#include "bearc.h"

int main(void) {
  char *input = "(567 + 3) * (4 / 6);";
  printf("input = %s\n", input);
  Token t = lex_start(input);
  while (t.kind != TK_EOF) {
    print_token(&t);
    printf("\n");
    t = lex_next(t);
  }
  printf("\n");
}
