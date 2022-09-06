CC=g++-9
FLAGS=-std=c++2a -Wall -pedantic -Wextra -Werror
TEST_LIBS=-lgtest_main -lgtest -lpthread

.PHONY: test clean

all: matrix.o test_bin

matrix.o: matrix.cpp matrix.hpp
	$(CC) $(FLAGS) matrix.cpp -c

test_bin: matrix.o test_matrix.cpp
	$(CC) $(FLAGS) matrix.o test_matrix.cpp -o test_bin $(TEST_LIBS)

test:
	./test_bin

valgrind:
	valgrind ./test_bin

clean:
	rm -f *.o test_bin