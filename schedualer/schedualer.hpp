#ifndef __SCHED__
#define __SCHED__

#include <queue>
#include <time.h>
#include <signal.h>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/chrono.hpp>
#include "../../cpp/DesignPatterns/observer/observer.hpp"
// using namespace boost::chrono;

namespace ilrd
{


  typedef std::pair<Dispatcher<int>* , boost::chrono::time_point<boost::chrono::system_clock>  > pair_t;
  typedef boost::chrono::time_point<boost::chrono::system_clock> time_point_t;
  class Timer
  {
  public:
    typedef void(*notifunction_t)(union sigval);
    Timer();
    Timer(notifunction_t , union sigval params );
    ~Timer();
    void Arm(boost::chrono::milliseconds millisec);
    long CheckDeadline();//return time remaining in  milliseconds
    void Disarm();
    // void ArmInterval();

  private:
    // long m_interval;
    timer_t m_tid;
    struct sigevent m_sev;
    struct itimerspec itime;
    struct itimerspec m_old_value;
  };

  class ComperFunc
  {
  public:
    bool operator()(const pair_t& lhs, const pair_t& rhs);
  };

  class Schedualer
  {
  public:
    Schedualer();
    void Add(CallbackBase<int>* cb, boost::chrono::milliseconds milli);
    static void Cycle(union sigval);

  private:
    // static bool CompFunc(pair_t lhs, pair_t rhs);
    boost::mutex m_mutex;
    std::priority_queue<pair_t, std::vector<pair_t>, ComperFunc > m_pq;
    Timer m_timer;
  };


}//namespace ilrd
#endif
