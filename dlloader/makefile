FLAGS = -ansi -pedantic-errors -Wall -Wextra -g

test:dll_test.out
so:soobject.so

soobject.so:soobject.cpp
	g++ $(FLAGS) soobject.cpp  -fPIC -shared -o soobject.so

dll_test.out:dlloader_test.cpp
	g++ $(FLAGS) dlloader_test.cpp dlloader.cpp sharedobject.cpp ../dirmonitor/dirmonitor.cpp -ldl -lboost_thread -lboost_system -o dll_test.out

.PHONY: clean
clean:
	rm -f *.so *.out *.gch *.o cfile cppfile

.PHONY: file
file: ./cpp.out ./c.out
	./cpp.out > ./cppfile
	./c.out > ./cfile
