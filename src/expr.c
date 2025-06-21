#include "bearc.h"

Expr *expr_group(Token op, Expr *inner) {
  Expr *expr = (Expr *)calloc(1, sizeof(Expr));
  expr->kind = EXPR_GROUP;
  expr->value.Group.op = op;
  expr->value.Group.expr = inner;
  return expr;
}

Expr *expr_binary(Token op, Expr *lhs, Expr *rhs) {
  Expr *expr = (Expr *)calloc(1, sizeof(Expr));
  expr->kind = EXPR_BINARY;
  expr->value.Binary.op = op;
  expr->value.Binary.lhs = lhs;
  expr->value.Binary.rhs = rhs;
  return expr;
}

Expr *expr_literal(Token tok) {
  Expr *expr = (Expr *)calloc(1, sizeof(Expr));
  expr->kind = EXPR_LITERAL;
  expr->value.Literal.tok = tok;
  expr->value.Literal.value.isize = atoi(tok.val);
  return expr;
}
