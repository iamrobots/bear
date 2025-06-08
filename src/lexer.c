#include "bearc.h"

static TokenKind map[] = {
    ['('] = TK_LPAREN,    [')'] = TK_RPAREN, ['+'] = TK_PLUS, ['-'] = TK_MINUS,
    ['/'] = TK_SLASH,     ['*'] = TK_SPLAT,  ['0'] = TK_NUM,  ['1'] = TK_NUM,
    ['2'] = TK_NUM,       ['3'] = TK_NUM,    ['4'] = TK_NUM,  ['5'] = TK_NUM,
    ['6'] = TK_NUM,       ['7'] = TK_NUM,    ['8'] = TK_NUM,  ['9'] = TK_NUM,
    [';'] = TK_SEMI_COLON};

Lexer lexer_new(char *input) {
  Lexer lexer;
  lexer.input = input;
  lexer.curr = input;
  return lexer;
}

Token next_token(Lexer *lexer) {
  char *curr = lexer->curr;

  while (*curr == ' ') {
    curr += 1;
  }

  Token t;
  t.kind = map[(unsigned char)*curr];
  t.val = curr;

  curr += 1;
  switch (t.kind) {
  case TK_EOF:
    return t;
  case TK_PLUS:
  case TK_MINUS:
  case TK_SLASH:
  case TK_SPLAT:
  case TK_LPAREN:
  case TK_RPAREN:
  case TK_SEMI_COLON:
    break;
  case TK_NUM:
    while (map[(unsigned char)*curr] == TK_NUM) {
      curr += 1;
    }
    break;
  }

  lexer->curr = curr;
  return t;
}
