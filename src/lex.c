#include "bearc.h"

void lexer_init(Lexer *lexer, char *input) {
  lexer->input = input;
  lexer->pos = 0;
}

void lexer_next(Lexer *lexer, Token *out) {
  int pos = lexer->pos;
  int start = pos;
  char *input = lexer->input;

  while (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\n' ||
         input[pos] == '\r') {
    pos += 1;
    start = pos;
  }

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
    out->num = input[pos] - '0';
    pos += 1;
    while ('0' <= input[pos] && input[pos] <= '9') {
      out->num *= 10;
      out->num += input[pos] - '0';
      pos += 1;
    }
    break;
  }

  out->pos = start;
  lexer->pos = pos;
}
