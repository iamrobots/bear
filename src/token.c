#include "bearc.h"
#include <stdio.h>

static char *map[] = {
    [TK_EOF] = "EOF",  [TK_PLUS] = "+",       [TK_MINUS] = "-",
    [TK_SLASH] = "/",  [TK_SPLAT] = "*",      [TK_LPAREN] = "(",
    [TK_RPAREN] = ")", [TK_SEMI_COLON] = ";", [TK_INT] = "INT"};

static int is_num(char c);
static void print_num(Token *token);

static int is_num(char c) { return '0' <= c && c <= '9' ? 1 : 0; }

static void print_num(Token *token) {
  char *input = token->val;
  while (is_num(*input)) {
    putchar(*input);
    input += 1;
  }
}

void print_token(Token *token) {
  switch (token->kind) {
  case TK_INT:
    print_num(token);
    break;
  default:
    printf("%s", map[token->kind]);
    break;
  }
}
