#include "bearc.h"
#include <stdio.h>
#include <stdlib.h>

ExprRef expr_error(ExprPool *pool, Token token, ExprRef ref) {
  Expr expr;
  expr.kind = EX_ERR;
  expr.token = token;
  expr.value.grouping = ref;
  return expr_pool_push(pool, expr);
}
ExprRef expr_grouping(ExprPool *pool, Token token, ExprRef inner) {
  Expr expr;
  expr.kind = EX_GROUPING;
  expr.token = token;
  expr.value.grouping = inner;
  return expr_pool_push(pool, expr);
}

ExprRef expr_binary(ExprPool *pool, Token token, ExprRef lhs, ExprRef rhs) {
  Expr expr;
  expr.kind = EX_BINARY;
  expr.token = token;
  expr.value.binary.lhs = lhs;
  expr.value.binary.rhs = rhs;
  return expr_pool_push(pool, expr);
}

ExprRef expr_literal(ExprPool *pool, Token token) {
  Expr expr;
  expr.kind = EX_LITERAL;
  expr.token = token;
  switch (token.kind) {
  case TK_NUM:
    expr.value.literal.usize = token.value.integer;
    break;
  default:
    printf("Invalid literal");
    abort();
    break;
  }

  return expr_pool_push(pool, expr);
}

void expr_pool_init(ExprPool *pool) {
  pool->len = 0;
  pool->cap = 8;
  pool->buff = (Expr *)calloc(pool->cap, sizeof(Expr));
}

ExprRef expr_pool_push(ExprPool *pool, Expr expr) {
  ExprRef ref = pool->len;
  if (pool->len == pool->cap) {
    pool->cap *= 2;
    pool->buff = realloc(pool->buff, sizeof(Expr) * pool->cap);
  }

  pool->buff[ref] = expr;
  pool->len += 1;
  return ref;
}

Expr *expr_pool_get(ExprPool *pool, ExprRef ref) {
  Expr *expr = &pool->buff[ref];
  return expr;
}

void expr_pool_free(ExprPool *pool) {
  if (pool->buff == NULL) {
    return;
  }

  free(pool->buff);
  pool->buff = NULL;
}

void pretty_print(ExprPool *pool, ExprRef ref) {
  Expr *expr = expr_pool_get(pool, ref);
  switch (expr->kind) {
  case EX_ERR:
    printf("ERROR");
    break;
  case EX_LITERAL:
    printf("%d", expr->value.literal.usize);
    break;
  case EX_BINARY:
    pretty_print(pool, expr->value.binary.lhs);
    switch (expr->token.kind) {
    case TK_PLUS:
      printf(" + ");
      break;
    case TK_MINUS:
      printf(" - ");
      break;
    case TK_STAR:
      printf(" * ");
      break;
    case TK_SLASH:
      printf(" / ");
      break;
    default:
      printf(" ??? ");
      break;
    }
    pretty_print(pool, expr->value.binary.rhs);
    break;
  case EX_GROUPING:
    printf("(");
    pretty_print(pool, expr->value.grouping);
    printf(")");
    break;
  }
}
