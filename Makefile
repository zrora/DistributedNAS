FLAGS = -ansi -pedantic-errors -Wall -Wextra -g
CXX = g++

all: threadpool.o sharedobject.o dlloader.o dirmonitor.o nbdproxy.o \
	request.o requesthandler.o memory.o libsingletone_instance.so

test:nbd_test.out

so:libread.so
#
# bdproxy.o: bdproxy.cpp bdproxy.hpp
# 	$(CXX) $(FLAGS) bdproxy.cpp -c -o $@

nbdproxy.o: nbdproxy.cpp nbdproxy.hpp
	$(CXX) $(FLAGS) nbdproxy.cpp -c -o $@

request.o: request.cpp request.hpp
	$(CXX) $(FLAGS) request.cpp -c -o $@

requesthandler.o: requesthandler.hpp requesthandler.cpp
	$(CXX) $(FLAGS) requesthandler.cpp -c -o $@

memory.o: memory.hpp memory.cpp
	$(CXX) $(FLAGS) memory.cpp -c -o $@
#
# callback.o: callback.hpp callback.cpp
# 	$(CXX) $(FLAGS) callback.cpp -c -o $@

libsingletone_instance.so:libsingletone_instance.cpp
	$(CXX) $(FLAGS) libsingletone_instance.cpp -fPIC -shared -o $@ -lpthread

# task.o: task/task.hpp
# 	$(CXX) $(FLAGS) ./task/task.cpp -c -o $@

threadpool.o: ./../cpp/DesignPatterns/ThreadPool/threadpool.hpp
	$(CXX) $(FLAGS) ./../cpp/DesignPatterns/ThreadPool/threadpool.cpp -c -o $@

dlloader.o:  ./dlloader/dlloader.cpp
	$(CXX) $(FLAGS) ./dlloader/dlloader.cpp -c -o $@

dirmonitor.o: ./dirmonitor/dirmonitor.cpp
	$(CXX) $(FLAGS) ./dirmonitor/dirmonitor.cpp -c -o $@

sharedobject.o: ./dlloader/sharedobject.cpp
	$(CXX) $(FLAGS) ./dlloader/sharedobject.cpp -c -o $@ -ldl

libread.so: ./pnpcommands/read.cpp libsingletone_instance.so
	$(CXX) $(FLAGS) ./pnpcommands/read.cpp requesthandler.cpp -fPIC -shared -o $@ \
	-L. -lsingletone_instance -ldl -lboost_system

nbd_test.o: nbd_test.cpp nbdproxy.o callback.o request.o \
	requesthandler.o memory.o libsingletone_instance.so
	$(CXX) $(FLAGS) nbd_test.cpp -c -o $@

nbd_test.out: requesthandler.o threadpool.o memory.o nbd_test.o\
	nbdproxy.o request.o dirmonitor.o dlloader.o sharedobject.o
	$(CXX) $(FLAGS) requesthandler.o threadpool.o memory.o nbd_test.o\
	 	dirmonitor.o sharedobject.o dlloader.o nbdproxy.o request.o -o $@\
	  -lpthread -lboost_thread -lboost_chrono -ldl -lboost_system \
		-L. -lsingletone_instance -Wl,-rpath=.

.PHONY: clean
clean:
	rm -f *.so *.out *.gch *.o cfile cppfile

.PHONY: cleanso
cleanso:
	rm ./pnpcommands/*.so

.PHONY: file
file: ./cpp.out ./c.out
	./cpp.out > ./cppfile
	./c.out > ./cfile
