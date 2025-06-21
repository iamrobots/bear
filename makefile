CFLAGS=-Wall -Wextra -pedantic

all: bearc

test: bearc_tests

clean:
	rm obj/*.o

bearc_tests: obj/tests.o obj/lex.o obj/utils.o
	cc $(CFLAGS) -o $@ $^

bearc: obj/lex.o obj/main.o
	cc $(CFLAGS) -o $@ $^

obj/lex.o: src/lex.c
	cc $(CFLAGS) -c -o $@ $^

obj/main.o: src/main.c
	cc $(CFLAGS) -c -o $@ $^

obj/tests.o: test/tests.c
	cc $(CFLAGS) -c -o $@ $^

obj/utils.o: test/utils.c
	cc $(CFLAGS) -c -o $@ $^
