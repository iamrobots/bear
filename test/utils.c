#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

void assert_eq(int expected, int actual, char *text_expected,
               char *text_actual) {
  if (expected == actual) {
    printf("PASS: %s == %s, expected: %d, actual: %d\n", text_expected,
           text_actual, expected, actual);
    return;
  } else {
    printf("FAIL: %s == %s, expected: %d, actual: %d\n", text_expected,
           text_actual, expected, actual);
  }
}
