#include "kamikaze.hpp"
#include <iostream>

using namespace ilrd;
void Func()
{
  std::cout << "Func is working" << '\n';
}

int main()
{
  boost::chrono::milliseconds ms(500);
  Kamikaze* kami = new Kamikaze(Func,ms);
  sleep(1);
  return 0;
}
