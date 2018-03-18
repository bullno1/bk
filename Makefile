.PHONY: test all clean

CFLAGS += \
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
		  -Wno-missing-field-initializers \
		  -DBK_DYNAMIC=1 # To check declaration

all: test

clean:
	rm -rf bin

test: bin/tests
	bin/tests

bin/bk.so: src/*.c
	mkdir -p bin
	$(CC) $(CFLAGS) $^ -fPIC -shared -fvisibility=hidden -DBK_BUILD=1 -o $@

bin/tests: tests/*.c tests/munit/munit.c bin/bk.so
	mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@
