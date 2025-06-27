#include "bearc.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  Parser parser = {0};
  ExprRef ref = 0;
  if (argc < 2) {
    printf("bearc [OPTIONS]");
    return 1;
  }
  parser_init(&parser, argv[1]);
  ref = parser_parse(&parser);
  pretty_print(&parser.pool, ref);
  printf("\n");
}
