#include "singletone.hpp"
// #include "handleton.h"
using namespace ilrd ;

void DoSomthing()
{
  // IMPLEMENT_SINGLETON(int);
  //
  Singleton<int> a;
  int *b = a.instance();
  std::cout <<"the address from DoSomthing function Object is:"<< b << '\n';

}
