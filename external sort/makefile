CC=g++-9
FLAGS=-std=c++2a -Wall -pedantic -Wextra -Werror
TEST_LIBS=-lgtest_main -lgtest -lpthread

.PHONY: test clean

all: clean sort.o test_bin

sort.o: sort.cpp sort.hpp
	$(CC) $(FLAGS) sort.cpp -c

test_bin: sort.o test_sort.cpp
	$(CC) $(FLAGS) sort.o test_sort.cpp -o test_bin $(TEST_LIBS)

test:
	./test_bin

valgrind:
	valgrind ./test_bin

clean:
	rm -f *.o test_bin