#include "bearc.h"

static TokenKind map[] = {
    ['('] = TK_LPAREN,     [')'] = TK_RPAREN, ['+'] = TK_PLUS, ['-'] = TK_MINUS,
    ['/'] = TK_SLASH,      ['*'] = TK_SPLAT,  ['0'] = TK_INT,  ['1'] = TK_INT,
    ['2'] = TK_INT,        ['3'] = TK_INT,    ['4'] = TK_INT,  ['5'] = TK_INT,
    ['6'] = TK_INT,        ['7'] = TK_INT,    ['8'] = TK_INT,  ['9'] = TK_INT,
    [';'] = TK_SEMI_COLON, ['.'] = TK_DOT};

static unsigned int is_whitespace(char c) {
  switch (c) {
  case ' ':
  case '\t':
  case '\n':
  case '\r':
  case '\v':
  case '\f':
    return 1;
  }

  return 0;
}

Token lex_next(Token token) {
  if (token.kind == TK_EOF) {
    return token;
  }
  return lex_start(token.val + token.len);
}

Token lex_start(char *input) {
  char *start = input;
  unsigned char len = 0;
  Token t;
  if (*start == '\0') {
    t.kind = TK_EOF;
    return t;
  }

  while (is_whitespace(*start)) {
    start += 1;
  }

  TokenKind kind;
  kind = map[(unsigned char)*start];
  len += 1;
  switch (kind) {
  case TK_EOF:
  case TK_PLUS:
  case TK_MINUS:
  case TK_SLASH:
  case TK_SPLAT:
  case TK_LPAREN:
  case TK_RPAREN:
  case TK_SEMI_COLON:
  case TK_DOT:
    break;
  case TK_INT:
    while (map[(unsigned char)*(start + len)] == TK_INT) {
      len += 1;
    }
    if (map[(unsigned char)*(start + len)] == TK_DOT) {
      kind = TK_FLOAT;
      len += 1;
      while (map[(unsigned char)*(start + len)] == TK_INT) {
        len += 1;
      }
    }
    break;
  case TK_FLOAT:
    break;
  }

  t.kind = kind;
  t.val = start;
  t.len = len;
  return t;
}

Token lex_number(char *input) { exit(1); }
