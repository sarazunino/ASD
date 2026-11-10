SRC   = src/priority_queue.c src/lib/hash_table.c src/main_ex3.c
TESTS = src/priority_queue.c src/lib/hash_table.c tests/test_ex3.c tests/unity.c
CFLAGS = -Iinclude -Itests -Isrc/lib

all:
	mkdir -p bin
	gcc $(CFLAGS) $(SRC) -o bin/main_ex3
	gcc $(CFLAGS) $(TESTS) -o bin/test_ex3

clean:
	rm -rf bin/