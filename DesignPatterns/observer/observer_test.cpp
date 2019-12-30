#include <iostream>
#include "observer.hpp"
using namespace ilrd;

enum mouse_states{NONE,LEFT,RIGHT,UP,DOWN};
enum terminal_states{WRITING,READING};
int id = 1;
int mouse_state = NONE;
int terminal_state = NONE;

class Mouse
{
public:
  Mouse():m_cb_mouse_state(*this, &Mouse::OnEvent, &Mouse::OnDeath){mouse_id = id++;}
  ~Mouse(){std::cout << "mouse dtor" << '\n';}
  void OnEvent(mouse_states ms){std::cout << "Mouse state = " << ms<<" id = "<< mouse_id << '\n';}
  void OnDeath()
  {
    std::cout << "Mouse Death" << '\n';

  }
  Callback<mouse_states, Mouse>& GetCallBack(){return m_cb_mouse_state;}
  void StopObserving(){m_cb_mouse_state.StopObserving();}
private:
  Callback<mouse_states, Mouse> m_cb_mouse_state;
  int mouse_id;
};

class Subject
{
public:
  Subject():m_mouse_state_disp()
  {}
  ~Subject(){}
/* message */
  void subscribe(CallbackBase<mouse_states>&cb_mouse)
                              {m_mouse_state_disp.Attach(&cb_mouse);}

  void unsubscribe(CallbackBase<mouse_states>&cb_mouse)
                              {m_mouse_state_disp.Detach(&cb_mouse);}

  void DetectMouseEvent(mouse_states ms)
                              {m_mouse_state_disp.NotifyAll(ms);}

private:
  Dispatcher<mouse_states> m_mouse_state_disp;

};

int main(int argc, char const *argv[])
{
  /************************************************************
  *********subscribe/unsubscribe and post a change**************
  *************************************************************/
//must declare publisher before observers to prevante valgrind errors
Subject mouse_publisher;
Mouse mouse1 ,mouse2, mouse3;

//subscribe to service
  mouse_publisher.subscribe(mouse1.GetCallBack());
  mouse_publisher.subscribe(mouse2.GetCallBack());
  mouse_publisher.subscribe(mouse3.GetCallBack());

// change of state sent to all subscribers
  mouse_publisher.DetectMouseEvent(LEFT);
  mouse_publisher.DetectMouseEvent(RIGHT);

//unsubscribe mouse 1
  std::cout <<'\n'<< "unsubscribing" << '\n';
// mouse_publisher.unsubscribe(mouse1.GetCallBack());
  mouse_publisher.DetectMouseEvent(UP);

  mouse1.StopObserving();
  mouse2.StopObserving();
  mouse3.StopObserving();
/************************************************************
**************                               **************
**********************************************************/


  return 0;
}
