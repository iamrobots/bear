CFLAGS=-std=c11 -Wall -Wextra -pedantic -g

all: bearc

test: bearc_tests

fmt:
	clang-format -i -- src/*.c src/*.h test/*.c test/*.h

clean:
	rm obj/*.o
	rm bearc
	rm bearc_tests

bearc_tests: obj/tests.o obj/lex.o obj/parser.o obj/expr.o obj/utils.o
	cc $(CFLAGS) -o $@ $^

bearc: obj/lex.o obj/parser.o obj/expr.o obj/main.o
	cc $(CFLAGS) -o $@ $^

obj/lex.o: src/lex.c
	cc $(CFLAGS) -c -o $@ $^

obj/expr.o: src/expr.c
	cc $(CFLAGS) -c -o $@ $^

obj/parser.o: src/parser.c
	cc $(CFLAGS) -c -o $@ $^

obj/main.o: src/main.c
	cc $(CFLAGS) -c -o $@ $^

obj/tests.o: test/tests.c
	cc $(CFLAGS) -c -o $@ $^

obj/utils.o: test/utils.c
	cc $(CFLAGS) -c -o $@ $^
