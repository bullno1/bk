.PHONY: test all clean

CFLAGS ?= \
		  -I include \
		  -g \
		  -O3 \
		  -fsanitize=undefined \
		  -fsanitize=address \
		  -std=c99 \
		  -pedantic \
		  -Wall \
		  -Werror \
		  -Wextra \
		  -Wno-missing-field-initializers

all: test

clean:
	rm -rf bin

test: bin/tests
	bin/tests

bin/tests: tests/*.c tests/munit/munit.c
	mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@
