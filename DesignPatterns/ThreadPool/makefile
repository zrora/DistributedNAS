CXX=g++#compiler to use

FLAGS=-ansi -Wall -Wextra -pedantic-errors -I.#universal compiler flags

all: threadpool_test.out

threadpool.o: threadpool.cpp
	$(CXX) threadpool.cpp $(FLAGS) -c -o threadpool.o

threadpool_test.o: threadpool_test.cpp
	$(CXX) threadpool_test.cpp -c $(FLAGS) -o threadpool_test.o

threadpool_test.out: threadpool_test.o threadpool.o
	$(CXX) threadpool_test.o threadpool.o $(FLAGS) -lboost_system -lboost_chrono -lboost_thread -o threadpool_test.out

.PHONY : clean
clean :
	rm -f *.o *.out
