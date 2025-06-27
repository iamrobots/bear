#include "bearc.h"
#include <stdio.h>

static Token empty_token = {0};

void lexer_init(Lexer *lexer, char *input) {
  lexer->input = input;
  lexer->pos = 0;
}

void lexer_next(Lexer *lexer, Token *out) {
  int pos = lexer->pos;
  int start = 0;
  char *input = lexer->input;
  *out = empty_token;

  while (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\n' ||
         input[pos] == '\r') {
    pos += 1;
  }

  start = pos;

  switch (input[pos]) {
  case '\0':
    out->kind = TK_EOF;
    out->pos = pos;
    return;
  case '(':
    out->kind = TK_LPAREN;
    pos += 1;
    break;
  case ')':
    out->kind = TK_RPAREN;
    pos += 1;
    break;
  case '+':
    out->kind = TK_PLUS;
    pos += 1;
    break;
  case '-':
    out->kind = TK_MINUS;
    pos += 1;
    break;
  case '/':
    out->kind = TK_SLASH;
    pos += 1;
    break;
  case '*':
    out->kind = TK_STAR;
    pos += 1;
    break;
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    out->kind = TK_NUM;
    out->value.num = input[pos] - '0';
    pos += 1;
    while ('0' <= input[pos] && input[pos] <= '9') {
      out->value.num *= 10;
      out->value.num += input[pos] - '0';
      pos += 1;
    }
    break;
  }

  out->pos = start;
  lexer->pos = pos;
}

void print_token(Token token) {
  printf("%d:", token.pos);
  switch (token.kind) {
  case TK_ERR:
    printf("TK_ERR\n");
    break;
  case TK_EOF:
    printf("TK_EOF\n");
    break;
  case TK_LPAREN:
    printf("TK_LPAREN\n");
    break;
  case TK_RPAREN:
    printf("TK_RPAREN\n");
    break;
  case TK_PLUS:
    printf("TK_PLUS\n");
    break;
  case TK_MINUS:
    printf("TK_MINUS\n");
    break;
  case TK_STAR:
    printf("TK_STAR\n");
    break;
  case TK_SLASH:
    printf("TK_SLASH\n");
    break;
  case TK_NUM:
    printf("TK_NUM(%d)\n", token.value.num);
    break;
  }
}
