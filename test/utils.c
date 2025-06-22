#include <stdio.h>

#include "utils.h"

void assert(int expected, char *text_expected) {
  if (!expected) {
    printf("FAIL: %s, expected: %d\n", text_expected, expected);
  }
}

void assert_eq(int expected, int actual, char *text_expected,
               char *text_actual) {
  if (expected != actual) {
    printf("FAIL: %s == %s, expected: %d, actual: %d\n", text_expected,
           text_actual, expected, actual);
  }
}
