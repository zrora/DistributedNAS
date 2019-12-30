FLAGS = -ansi -pedantic-errors -Wall -Wextra -g
test: kamikaze_test.out
so: libsingletone_sched.so

libsingletone_sched.so:
	g++ $(FLAGS) libsingletone_sched.cpp ../schedualer/schedualer.cpp -fPIC -shared -o $@\
	 -lboost_thread -lboost_system -lboost_chrono -lrt

kamikaze_test.out:  libsingletone_sched.so
	g++ $(FLAGS) kamikaze_test.cpp kamikaze.cpp ../schedualer/schedualer.cpp -o $@ \
	-lrt -ldl -lboost_chrono -lboost_system \
 	-L. -lsingletone_sched -Wl,-rpath=.

clean:
	rm -f *.so *.out *.out
# project/schedualer/schedualer.cpp
