CC=g++-9
FLAGS=-std=c++2a -Wall -pedantic -Wextra -Werror
TEST_LIBS=-lgtest_main -lgtest -lpthread

.PHONY: test clean

all: clean serializer.o test_bin

serializer.o: serializer.cpp serializer.hpp serializer.tpp
	$(CC) $(FLAGS) serializer.cpp -c

test_bin: serializer.o test_serializer.cpp
	$(CC) $(FLAGS) serializer.o test_serializer.cpp -o test_bin $(TEST_LIBS)

test:
	./test_bin

valgrind:
	valgrind ./test_bin

clean:
	rm -f *.o test_bin