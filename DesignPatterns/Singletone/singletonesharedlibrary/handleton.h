#ifndef __HANDLETON_H__
#define __HANDLETON_H__

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
/*

Usage:
void f(Something &s){...}

void letsusesomething()
{
	Handleton<Something> s;
	s->method();
	f(s);
}

void useitagain()
{
	Handleton<Something> s;
	s->someothermethod();
}

*/

#ifdef WIN32
#define NO_STATIC_PRAGMA _Pragma("comment(lib,\"xxx.lib\")")
#elif defined(WIN64)
#define NO_STATIC_PRAGMA _Pragma("comment(lib,\"xxx.lib\")")
#else
#define NO_STATIC_PRAGMA _Pragma("message(\"please ensure that \"__FILE__\
\" is compiled into a shared object and not a static library\")")
#endif

namespace ilrd{
#define Atomic_Or(T,X) __sync_fetch_and_or(T, X)

#define IMPLEMENT_SINGLETON(T) 						\
 NO_STATIC_PRAGMA  							\
template<> 								\
T *&Handleton<T>::GetInstance() 					\
{ 									\
        static volatile bool sv_isReady=0; 				\
	static volatile int sv_shouldInit=0; 				\
	static T *theObject = 0; 					\
	if(!sv_isReady) 						\
	{ 								\
		if(!Atomic_Or(&sv_shouldInit, true)) 				\
		{							\
			theObject = new T(); 				\
			::atexit(&Handleton<T>::DestroyInstance); 	\
			Atomic_Or(&sv_isReady,true); 			\
		} 							\
		while(!sv_isReady) 					\
		{ 							\
			::sleep(0); 					\
		} 							\
	}								\
	return theObject;						\
}

template <class T>
class Handleton
{
public:
  Handleton() { GetInstance(); }
  operator T &() { return *GetInstance(); }
  operator const T &() const { return *GetInstance(); }
  T *operator->(){ return GetInstance(); }
  const T *operator->() const { return GetInstance(); }

private:
  T *&GetInstance();

  static void DestroyInstance()
  {
    Handleton<T> h;
    T *ptr = h.GetInstance();
    h.GetInstance() = reinterpret_cast<T*>(0xdeadbeef);
    __sync_synchronize();
    delete ptr;
  }
};

}//namespace ilrd

#endif
