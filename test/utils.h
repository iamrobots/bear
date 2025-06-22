#define ASSERT(a) assert_eq(a, #a);
#define ASSERT_EQ(a, b) assert_eq(a, b, #a, #b);

void assert(int expected, char *text_expected);
void assert_eq(int expected, int actual, char *text_expected,
               char *text_actual);
