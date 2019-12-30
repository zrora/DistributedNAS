#ifndef __KAMIKAZE_HPP__
#define __KAMIKAZE_HPP__

#include <boost/function.hpp>
#include <boost/chrono.hpp>
#include<boost/bind.hpp>
// #include "../schedualer/schedualer.hpp"
#include "../../cpp/DesignPatterns/observer/observer.hpp"


namespace ilrd
{
class Kamikaze
{
public:
  Kamikaze(boost::function<void()> func, boost::chrono::milliseconds);
  void OnEvent(int);
  void OnDeath();

private:
  class KamikazeDispatcher;
  Callback<int, Kamikaze> m_cb;
  boost::function<void()> m_func;
  //object can only be created on the heap
  ~Kamikaze();
};

/****************************
KamikazeDispatcher
****************************/
class Kamikaze::KamikazeDispatcher
{
public:
  KamikazeDispatcher(Callback<int, Kamikaze>& cb);
  void Subscribe(Callback<int, Kamikaze> &observer);
  void UnSubscribe(Callback<int, Kamikaze> &observer);
  void Notify(int);

private:
  Dispatcher<int> m_kamikaze_dis;
};



}//namespaceilrd
#endif//__KAMIKAZE_HPP__
