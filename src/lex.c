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
    out->pos = pos;
    out->kind = TK_EOF;
    return;
  case '(':
    pos += 1;
    out->kind = TK_LPAREN;
    break;
  case ')':
    pos += 1;
    out->kind = TK_RPAREN;
    break;
  case '+':
    pos += 1;
    out->kind = TK_PLUS;
    break;
  case '-':
    pos += 1;
    out->kind = TK_MINUS;
    break;
  case '/':
    pos += 1;
    out->kind = TK_SLASH;
    break;
  case '*':
    pos += 1;
    out->kind = TK_STAR;
    break;
  case ';':
    pos += 1;
    out->kind = TK_SEMICOLON;
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
    out->value.integer = input[pos] - '0';
    pos += 1;
    while ('0' <= input[pos] && input[pos] <= '9') {
      out->value.integer *= 10;
      out->value.integer += input[pos] - '0';
      pos += 1;
    }
    break;
  }

  out->pos = start;
  lexer->pos = pos;
}
