CC=g++-9
FLAGS=-std=c++2a -Wall -pedantic -Wextra -Werror
TEST_LIBS=-lgtest_main -lgtest -lpthread

.PHONY: test clean

all: bigint.o test_bin

bigint.o: bigint.cpp bigint.hpp
	$(CC) $(FLAGS) bigint.cpp -c

test_bin: bigint.o test_bigint.cpp
	$(CC) $(FLAGS) bigint.o test_bigint.cpp -o test_bin $(TEST_LIBS)

test:
	./test_bin

valgrind:
	valgrind ./test_bin

clean:
	rm -f *.o test_bin