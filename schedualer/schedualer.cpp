// #include <boost/chrono.hpp>
// #include <boost/chrono.hpp>
#include <queue>
#include <boost/function.hpp>
#include <iostream>
#include "schedualer.hpp"


namespace ilrd
{
  Schedualer::Schedualer()
  {
    union sigval params;
    //set a pointer to this as a parameter
    params.sival_ptr = this;
    Timer timer(Schedualer::Cycle, params);
    m_timer = timer;
  }

  void Schedualer::Add(CallbackBase<int>* cb, boost::chrono::milliseconds milli)
  {
    boost::unique_lock<boost::mutex> ul(m_mutex);
    //create a pair to add to pq
    time_point_t tp = boost::chrono::system_clock::now() + milli;
    // std::cout << "schecualer Add tp = " <<tp<< '\n';
    Dispatcher<int>* tempdis = new(Dispatcher<int>);
    tempdis->Attach(cb);
    pair_t pq_pair(tempdis, tp);
    m_pq.push(pq_pair);

    //create a temp now time+ milisec to check against check deaeline
    //timer is schedualed for a mission that has a longer deadline)
    // std::cout << "deadline" << m_timer.CheckDeadline()<< "MILLI"<<milli.count() <<'\n';
    if(m_timer.CheckDeadline() > milli.count() || m_timer.CheckDeadline() == 0)
    {
        m_timer.Arm(milli);
    }
  }
  //
  void Schedualer::Cycle(union sigval sig)
  {
    //sig will always send the Schedualer address this;
    Schedualer *temp_sched = reinterpret_cast<Schedualer*>(sig.sival_ptr);
    pair_t pq_pair = temp_sched->m_pq.top();
    temp_sched->m_pq.pop();


    //activate function in pair;
    pq_pair.first->NotifyAll(0);
    delete pq_pair.first;
    pq_pair.first = NULL;

    pq_pair = temp_sched->m_pq.top();
    // schadual the next mission
    // boost::chrono::milliseconds ms = pq_pair.second - boost::chrono::system_clock::now();
    if(!temp_sched->m_pq.empty())
    {
      // std::cout <<"timer diff: " << pq_pair.second - boost::chrono::system_clock::now() << '\n';
      temp_sched->m_timer.Arm(boost::chrono::duration_cast<boost::chrono::milliseconds>(pq_pair.second - boost::chrono::system_clock::now() ));
      // std::cout << "Cycle:next time in timer: " <<temp_sched->m_timer.CheckDeadline()<< '\n';
    }
  }

  Timer::Timer()
  {}

  Timer::Timer(notifunction_t notifunciton , union sigval params )
  {

    m_sev.sigev_notify = SIGEV_THREAD;
    m_sev.sigev_notify_function = notifunciton;
    // m_sev.sigev_signo = 0;
    m_sev.sigev_notify_attributes = NULL;
    m_sev.sigev_value = params;

    timer_create(CLOCK_REALTIME, &m_sev, &m_tid);

  }

  void Timer::Arm(boost::chrono::milliseconds ms)
  {
    boost::chrono::milliseconds zeroms(0);

    if(ms > zeroms)
    {

      itime.it_value.tv_sec = ms.count() / 1000;
      itime.it_value.tv_nsec = (ms.count() % 1000) * 1000000;
      std::cout << "/* TIMER WILL BE SET TO : */" <<itime.it_value.tv_sec<< '\n';
    }
//if the time sent is negative or zero then the next activation will be in 1 nanosec
    else
    {
      itime.it_value.tv_sec = 0;
      itime.it_value.tv_nsec = 1;
    }

    itime.it_interval.tv_sec = 0;
    itime.it_interval.tv_nsec = 0;

    timer_settime(m_tid, 0, &itime, &m_old_value) ;
  }

  Timer::~Timer()
  {
      // Disarm();
      // timer_delete(m_tid);
  }

  void Timer::Disarm()
  {
    itime.it_value.tv_nsec = 0;
    itime.it_value.tv_sec = 0;

    timer_settime(m_tid, 0, &itime, &m_old_value);
  }

 long Timer::CheckDeadline()
 {
   //getttime returns the time until next experation
   timer_gettime(m_tid, &m_old_value);
   return (((m_old_value.it_value.tv_nsec) / 1000000) +  m_old_value.it_value.tv_sec );
 }

bool ComperFunc::operator()(const pair_t& lhs, const pair_t& rhs)
{
  return (lhs.second > rhs.second);
}

}//namespace ilrd
