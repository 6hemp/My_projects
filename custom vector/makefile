CC=g++-9
FLAGS=-std=c++2a -Wall -pedantic -Wextra -Werror
TEST_LIBS=-lgtest_main -lgtest -lpthread

.PHONY: test clean

all: clean vect.o test_bin

vect.o: vect.cpp vect.hpp vect.tpp
	$(CC) $(FLAGS) vect.cpp -c

test_bin: vect.o test_vect.cpp
	$(CC) $(FLAGS) vect.o test_vect.cpp -o test_bin $(TEST_LIBS)

test:
	./test_bin

valgrind:
	valgrind ./test_bin

clean:
	rm -f *.o test_bin