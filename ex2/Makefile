SRC   = src/hash_table.c src/main_ex2.c
TESTS = src/hash_table.c tests/test_ex2.c tests/unity.c
CFLAGS = -Iinclude -Itests

all:
	mkdir -p bin
	gcc $(CFLAGS) $(SRC) -o bin/main_ex2
	gcc $(CFLAGS) $(TESTS) -o bin/test_ex2

clean:
	rm -rf bin/