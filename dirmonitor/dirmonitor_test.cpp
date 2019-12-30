#include "dirmonitor.hpp"
using namespace ilrd;
int id = 0;

class Observer
{
public:
  Observer(DirMonitor& dm):observer_id(++id),m_cb_observer(*this, &Observer::OnEvent,&Observer::OnDeath)
  {
    dm.subscribe(m_cb_observer);
    std::cout << "observer created" << '\n';
  }

  ~Observer()
  {
    std::cout << "destructor was called" << '\n';
  }

  void OnEvent(std::string ev)
  {
    std::cout << "state was changed id = " <<observer_id<< ev<< '\n';
  }

  void OnDeath()
  {
    std::cout << "dying" << '\n';
  }

  Callback<std::string, Observer>& GetCallBack()
  {
    return m_cb_observer;
  }

private:
  int observer_id;
  Callback<std::string, Observer> m_cb_observer;
};

int main(int argc, char const *argv[])
{

  DirMonitor dm("/home/bob45/svn/Proj/src/project/",  IN_MODIFY | IN_CREATE | IN_DELETE);
  Observer obs1(dm) , obs2(dm), obs3(dm);
  dm.Monitor();
  while(1);

  return 0;
}
