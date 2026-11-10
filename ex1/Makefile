SRC   = src/hybridsort.c src/record.c src/main_ex1.c
TESTS = src/hybridsort.c src/record.c tests/test_ex1.c tests/unity.c
CFLAGS = -Iinclude -Itests

all:
	mkdir -p bin
	gcc $(CFLAGS) $(SRC) -o bin/main_ex1
	gcc $(CFLAGS) $(TESTS) -o bin/test_ex1

clean:
	rm -rf bin/