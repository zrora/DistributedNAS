#include <iostream>
#include "schedualer.hpp"

void TestFunction()
{
  // hey.sival_int = 5;
  std::cout << "Testfunction is working" << '\n';
}
void TestFunction2()
{
  // hey.sival_int = 5;

  std::cout << "Testfunction2 is working" << '\n';
}
void TestFunction3()
{
  // hey.sival_int = 5;

  std::cout << "Testfunction3 is working" << '\n';
}

using namespace ilrd;

int main()
{

  // Timer time(TestFunction);
    std::cout << "arming" << '\n';
    boost::chrono::milliseconds ms(1000);
    boost::chrono::milliseconds ms2(2000);
    boost::chrono::milliseconds ms3(4000);
    boost::chrono::milliseconds ms4(8000);
    boost::chrono::milliseconds ms5(16000);
  // time.Arm(ms);
  // std::cout << "infinite loop" << '\n';
  // while(time.CheckDeadline()!= 0)
  // {
  // };
  // std::cout << "out of loop" << '\n';

  Schedualer sched;
  sched.Add(TestFunction, ms);
  // sched.Add(TestFunction2, ms2);
  // sched.Add(TestFunction3, ms3);
  // sched.Add(TestFunction2, ms4);
  // sched.Add(TestFunction, ms5);
  // sleep(3);
  // std::cout << "out of sleep" << '\n';
  // sched.Add(TestFunction, ms);

  while(1);

  return 0;
}
