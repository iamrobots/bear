#include "bearc.h"
#include <stdio.h>
#include <stdlib.h>

ExprRef expr_error(ExprPool *pool, Token token, ExprRef ref) {
  Expr expr = {.kind = EX_ERR, .token = token, .inner = ref};
  return expr_pool_push(pool, expr);
}
ExprRef expr_grouping(ExprPool *pool, Token token, ExprRef inner) {
  Expr expr = {.kind = EX_GROUPING, .token = token, .inner = inner};
  return expr_pool_push(pool, expr);
}

ExprRef expr_binary(ExprPool *pool, Token token, ExprRef lhs, ExprRef rhs) {
  Expr expr;
  expr.kind = EX_BINARY;
  expr.token = token;
  expr.binary.lhs = lhs;
  expr.binary.rhs = rhs;
  return expr_pool_push(pool, expr);
}

ExprRef expr_literal(ExprPool *pool, Token token) {
  Expr expr;
  expr.kind = EX_LITERAL;
  expr.token = token;
  switch (token.kind) {
  case TK_NUM:
    expr.number = token.num;
    break;
  default:
    printf("Invalid literal");
    abort();
    break;
  }

  ExprRef ref = expr_pool_push(pool, expr);
  return ref;
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

void print_expr(ExprPool *pool, ExprRef ref) {
  Expr expr = *expr_pool_get(pool, ref);
  switch (expr.kind) {
  case EX_ERR:
    printf("EX_ERR\n");
    printf("  token: ");
    print_token(expr.token);
    break;
  case EX_LITERAL:
    printf("EX_LITERAL(%d)\n", expr.number);
    printf("  token: ");
    print_token(expr.token);
    break;
  case EX_BINARY:
    printf("EX_BINARY\n");
    printf("  token: ");
    print_token(expr.token);
    print_expr(pool, expr.binary.lhs);
    print_expr(pool, expr.binary.rhs);
    break;
  case EX_GROUPING:
    printf("EX_GROUPING\n");
    printf("  token: ");
    print_token(expr.token);
    print_expr(pool, expr.inner);
    break;
  }
}
