CC=g++-9
FLAGS=-std=c++2a -Wall -pedantic -Wextra -Werror
TEST_LIBS=-lgtest_main -lgtest -lpthread

.PHONY: test clean

all: clean format.o test_bin

format.o: format.cpp format.hpp format.tpp
	$(CC) $(FLAGS) format.cpp -c

test_bin: format.o test_format.cpp
	$(CC) $(FLAGS) format.o test_format.cpp -o test_bin $(TEST_LIBS)

test:
	./test_bin

valgrind:
	valgrind ./test_bin

clean:
	rm -f *.o test_bin