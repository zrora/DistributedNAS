CXX=g++#compiler to use

FLAGS=-ansi -Wall -Wextra -pedantic-errors -I.#universal compiler flags

waitable_test.o: watiable_test.cpp
	$(CXX) @.cpp $(FLAGS) -c @.cpp -o @.o

waitable_test.out: watiable_test.o
	$(CXX) @.cpp $(FLAGS) -lboost_system -lboost_chrono -lboost_thread @.o @.out

.PHONY : clean
clean :

    rm -f *.o *.out
