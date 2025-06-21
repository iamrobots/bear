#define ASSERT_EQ(a, b) assert_eq(a, b, #a, #b);

void assert_eq(int expected, int actual, char* text_expected, char* text_actual);
