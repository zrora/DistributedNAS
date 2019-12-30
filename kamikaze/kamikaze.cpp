#include "kamikaze.hpp"
#include "../../cpp/DesignPatterns/Singletone/singletonesharedlibrary/singletone.hpp"
#include "../schedualer/schedualer.hpp"
namespace ilrd
{
  /****************************
  Kamikaze
  ****************************/
  Kamikaze::Kamikaze(boost::function<void()> func, boost::chrono::milliseconds ms):
                                            m_cb(*this, &Kamikaze::OnEvent, &Kamikaze::OnDeath),
                                            m_func(func)
  {
    //create a dispatcher and register m_cb
    // boost::shared_ptr<KamikazeDispatcher> sp_disp (new KamikazeDispatcher(m_cb));
    // KamikazeDispatcher* sp_disp (new KamikazeDispatcher(m_cb));
    //register dispatcher notify function to schedualer
    Singleton<Schedualer> sched;
    sched.instance()->Add(&m_cb, ms);

  }

  void Kamikaze::OnEvent(int)
  {
    m_func();
    delete this;
  }

  void Kamikaze::OnDeath()
  {
    std::cout << "Kamikaze::OnDeath" << '\n';;
  }

  Kamikaze::~Kamikaze(){}

  /****************************
  KamikazeDispatcher
  ****************************/
  void Kamikaze::KamikazeDispatcher::Subscribe(Callback<int, Kamikaze> &observer)
  {
    m_kamikaze_dis.Attach(&observer);
  }
  void Kamikaze::KamikazeDispatcher::UnSubscribe(Callback<int, Kamikaze> &observer)
  {
    m_kamikaze_dis.Detach(&observer);
  }
  void Kamikaze::KamikazeDispatcher::Notify(int event)
  {
    m_kamikaze_dis.NotifyAll(event);
  }

  Kamikaze::KamikazeDispatcher::KamikazeDispatcher(Callback<int, Kamikaze>& cb)
  {
    Subscribe(cb);
  }


}//namespace ilrd
