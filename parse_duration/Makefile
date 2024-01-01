CFLAGS ?= -flto -O3 -std=c99 -pedantic -Wall -Werror

test: parse_duration_test.c parse_duration.h
	$(CC) $(CFLAGS) parse_duration_test.c -o test -lcriterion
	valgrind ./test
