#include <iostream>
// #include <pthread.h>
#include "singletone.hpp"
// #include "handleton.h"
// #include <boost/atomic.hpp>
// #include <boost/thread/mutex.hpp>

// using boost::atomic;
using namespace ilrd;
void test1(void);

int main()
{
  test1();
 // test2();

}

void test1(void)
{
  int *amir = Singleton<int>::instance();
  std::cout << "amir= " << *amir << '\n';
  std::cout << "amirs address = " << amir << '\n';
}

// void test2(void)
// {
//   pthread_t tid[10];
//
//   for (size_t i = 0; i < 10; i++)
//   {
//     pthread_create(&tid[i], NULL, threadapp, NULL);
//   }
//   // std::cout << "broadcasting"<< '\n';
//
//   // while (flag != 10)
//   // {
//   //   std::cout << flag << '\n';
//   // }
//   //
//   // flag = 20;
//   // pthread_cond_broadcast(&thread_cv);
//
//   for (size_t i = 9; i <= 0; i--)
//   {
//     pthread_join(tid[9], NULL);
//   }
//
// }
//
// void* threadapp(void* var)
// {
//   // while(flag != 20)
//   // {
//   //   ++flag;
//   //   pthread_cond_wait(&thread_cv, &container_access_mu);
//   // }
//
//   Singleton<int> a;
//
//   int* amir = a.instance();
//   *amir = *amir + 1;
//   std::cout << *amir << '\n';
//   std::cout << "singletoneaddress:" << amir<< '\n';
//
//   return amir;
// }
