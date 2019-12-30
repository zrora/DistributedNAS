#ifndef __OBSERVER_HPP__
#define __OBSERVER_HPP__ 1

#include <boost/noncopyable.hpp> // noncopyable
#include <list>     //std::list
#include <iostream>
namespace ilrd
{
template <class Event>
class Dispatcher;

template <class Event>
class CallbackBase
{
public:

    CallbackBase();
    virtual ~CallbackBase();
    friend class Dispatcher<Event>;
    virtual void Update(Event) = 0;
    virtual void StopObserving() = 0;
    void SetDispatcher(Dispatcher<Event>* dispatcher);

private:
    Dispatcher<Event>* m_dispatcher;
};

// holds callbacks for event and for subject destruction
template <class Event, class Owner>
class Callback: public CallbackBase<Event>
{
public:
    typedef void (Owner::*on_event_func)(Event event);
    typedef void (Owner::*on_subj_death_func)();

    explicit Callback(Owner& owner,
                      on_event_func on_event,
                      on_subj_death_func on_subj_death);
    ~Callback(){}
    void Update(Event);
    void StopObserving();

    void OnEvent(on_event_func);
    void OnSubjDeath(on_subj_death_func);

private:
    Owner& m_owner;
    on_event_func m_on_event;
    on_subj_death_func m_on_subj_death;
};

// dispatches event updates from the subject
template <class Event>
class Dispatcher: private boost::noncopyable
{
public:
    ~Dispatcher();
    void Attach(CallbackBase<Event>*);
    void Detach(CallbackBase<Event>*);
    void NotifyAll(Event);

private:
    std::list<CallbackBase<Event>* > m_subscribers;
};

template <class Event>
Dispatcher<Event>::~Dispatcher()
{
  typename std::list<CallbackBase<Event>*>::iterator it;
  it = m_subscribers.begin();
  for(;it != m_subscribers.end();++it)
  {
    (*it)->StopObserving();
    (*it)->SetDispatcher(0);
  }
}

template <class Event>
void Dispatcher<Event>::Attach(CallbackBase<Event>* cbb)
{
  cbb->SetDispatcher(this);
  m_subscribers.push_front(cbb);
}

template <class Event>
void Dispatcher<Event>::Detach(CallbackBase<Event>* cbb)
{
  typename std::list<CallbackBase<Event>*>::iterator it;
  it = m_subscribers.begin();
  for(;it != m_subscribers.end();++it)
  {
    std::cout << "searching" << '\n';
    if(*it == cbb)
    {
      std::cout << "found it" << '\n';
      (*it)->SetDispatcher(0);
      m_subscribers.erase(it);
      break;
    }
  }


  std::cout << "removing" << '\n';
}

template <class Event>
void Dispatcher<Event>::NotifyAll(Event event)
{
  typename std::list<CallbackBase<Event>*>::iterator it;
  it = m_subscribers.begin();
  for(;it != m_subscribers.end();it++ )
  {
    std::cout << " size is = "<<m_subscribers.size()<< '\n';
    (*it)->Update(event);
  }
}

template <class Event>
CallbackBase<Event>::CallbackBase():m_dispatcher(0){}

template <class Event>
CallbackBase<Event>::~CallbackBase()
{
// std::cout << "call back base dtor" << '\n';
  if(m_dispatcher)
  {
    std::cout << "/*detach*/" << '\n';
    m_dispatcher->Detach(this);
  }
}

template <class Event>
void CallbackBase<Event>::SetDispatcher(Dispatcher<Event>* dispatcher)
{
  m_dispatcher = dispatcher;
}

template <class Event, class Owner>
Callback<Event, Owner>::Callback(Owner& owner, on_event_func on_event, on_subj_death_func on_subj_death):
CallbackBase<Event>(), m_owner(owner),  m_on_event(on_event),m_on_subj_death(on_subj_death)
{}

template <class Event, class Owner>
void Callback<Event, Owner>::Update(Event event)
{
  (m_owner.*m_on_event)(event);
}

template <class Event, class Owner>
void Callback<Event, Owner>::StopObserving()
{
  (m_owner.*m_on_subj_death)();
}

template <class Event, class Owner>
void Callback<Event, Owner>::OnEvent(on_event_func o_e_f)
{
  m_on_event = o_e_f;
}

template <class Event, class Owner>
void Callback<Event, Owner>::OnSubjDeath(on_subj_death_func s_d_f)
{
  m_on_subj_death = s_d_f;
}

}//namespace ilrd
#endif// __OBSERVER_HPP__
