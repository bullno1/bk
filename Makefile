.PHONY: test all clean

CFLAGS += -I include -fsanitize=undefined -fsanitize=address

all: test

clean:
	rm -rf bin

test: bin/tests
	bin/tests

bin/tests: tests/*.c src/*.c tests/munit/munit.c
	mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@
