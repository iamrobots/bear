#include "bearc.h"
#include <stdio.h>
#include <stdlib.h>

static long eval2(ExprPool *pool, ExprRef ref);
static void eval3(ExprPool *pool, ExprRef ref);

void eval(ExprPool *pool, ExprRef ref) {
  long result = eval2(pool, ref);
  printf("# result = %ld\n\n", result);
  printf(".text\n");
  printf(".global _start\n");
  printf("_start:\n");
  eval3(pool, ref);
  printf("  movq %%rax, %%rdi\n");
  printf("  movq $0x2000001, %%rax\n");
  printf("  syscall\n");
}

static long eval2(ExprPool *pool, ExprRef ref) {
  long lhs = 0;
  long rhs = 0;
  Expr *expr = expr_pool_get(pool, ref);
  switch (expr->kind) {
  case EX_ERR:
    printf("Error: %d", expr->token.pos);
    exit(1);
    break;
  case EX_LITERAL:
    return expr->value.literal.usize;
  case EX_BINARY:
    lhs = eval2(pool, expr->value.binary.lhs);
    rhs = eval2(pool, expr->value.binary.rhs);
    switch (expr->token.kind) {
    case TK_PLUS:
      return lhs + rhs;
    case TK_MINUS:
      return lhs - rhs;
    case TK_STAR:
      return lhs * rhs;
    case TK_SLASH:
      return lhs / rhs;
    default:
      printf("Error: %d", expr->token.pos);
      exit(1);
      break;
    }
    break;
  case EX_GROUPING:
    return eval2(pool, expr->value.grouping);
  }
}

static void eval3(ExprPool *pool, ExprRef ref) {
  Expr *expr = expr_pool_get(pool, ref);
  switch (expr->kind) {
  case EX_ERR:
    printf("Error: %d\n", expr->token.pos);
    exit(1);
    break;
  case EX_LITERAL:
    printf("  movq $%d, %%rax\n", expr->value.literal.usize);
    break;
  case EX_BINARY:
    eval3(pool, expr->value.binary.lhs);
    printf("  movq %%rax, %%rcx\n");
    eval3(pool, expr->value.binary.rhs);
    switch (expr->token.kind) {
    case TK_PLUS:
      printf("  addq %%rcx, %%rax\n");
      break;
    case TK_MINUS:
      printf("  subq %%rcx, %%rax\n");
    case TK_STAR:
      printf("  mulq %%rcx\n");
      break;
    case TK_SLASH:
      printf("  divq %%rcx\n");
      break;
    default:
      printf("Error: %d\n", expr->token.pos);
      exit(1);
      break;
    }
    break;
  case EX_GROUPING:
    eval3(pool, expr->value.grouping);
    break;
  }
}
