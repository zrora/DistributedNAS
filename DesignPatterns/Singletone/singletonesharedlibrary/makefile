FLAGS = -ansi -pedantic-errors -Wall -Wextra -g
test: singletone_test.out
so: libsingletone.so

libsingletone.so: sharedobject.cpp
	g++ $(FLAGS) sharedobject.cpp -fPIC -shared -o $@ -lboost_thread -lboost_system

singletone_test.out: singletone_test.cpp libsingletone.so
	g++ $(FLAGS) singletone_test.cpp -o $@ -L. -lsingletone -Wl,-rpath=.

clean:
	rm -f *.so *.out *.out
