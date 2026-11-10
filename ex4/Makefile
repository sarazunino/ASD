SRC   = src/graph.c src/bfs.c src/main_ex4.c lib/hash_table.c
TESTS = src/graph.c src/bfs.c tests/test_ex4.c lib/hash_table.c
CFLAGS = -Iinclude -Itests -Ilib

all:
	mkdir -p bin
	gcc $(CFLAGS) $(SRC) -o bin/main_ex4
	gcc $(CFLAGS) $(TESTS) -o bin/test_ex4

clean:
	rm -rf bin/