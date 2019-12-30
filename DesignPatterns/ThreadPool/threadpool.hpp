

#ifndef __THREADPOOL
#define __THREADPOOL
#include "./waitablequeue/waitablequeue.hpp"
#include <boost/atomic/atomic.hpp>
#include <boost/chrono/include.hpp>
namespace ilrd
{
class ThreadFunctor;
class TaskBase;
class ThreadPool;
/***************************
*****Compare FUNCTOR**********
****************************/
class CompareFunctor
{
public:
  bool operator()(boost::shared_ptr<TaskBase> lhs, boost::shared_ptr<TaskBase> rhs);
};
/****************************
***********THREADPOOL********
****************************/
class ThreadPool
{
private:
  friend class ThreadFunctor;
  friend class CompareFunctor;
  friend class DeleteTask;
  friend class PauseTask;

public:
  ThreadPool(int num_of_threads);
  void AddTask(boost::shared_ptr<TaskBase> tb);
  void SetThreadNumber(int new_num_of_threads);
  void Pause();
  void Resume();
  void Stop(boost::chrono::milliseconds);//timeout var

private:
  void CreateThread();
  void LockCV();
  bool m_is_paused;

  boost::atomic<int> m_num_of_threads;
  WaitableQueue<boost::shared_ptr<TaskBase>, std::vector<boost::shared_ptr<TaskBase> >, CompareFunctor> m_wq;
  std::map<boost::thread::id, ThreadFunctor*> m_map;
  boost::mutex m_mutex;//protects map
  boost::condition_variable m_cv;//call when using pause
  boost::condition_variable m_stop_cv;
};



/***************************
*****THREAD FUNCTOR**********
****************************/
class ThreadFunctor
{
public:
  ThreadFunctor();
  void RegisterFunctor(ThreadPool* tp);
  void operator()(ThreadPool* tp);
  void KillMe();
private:
  bool i_am_alive;
};
/****************************
****TASK BASE AND DERIVED****
****************************/

enum priority {LOW, MEDIUM, HIGH};

class TaskBase
{

public:
  friend class PoolTask;
  TaskBase(enum priority = LOW);
  virtual ~TaskBase() = 0;
  virtual void operator()() = 0;
  int GetPriority();
private:
  int m_priority;
};

class PoolTask:public TaskBase
{
public:
  PoolTask(ThreadPool& tp, int pri = 4):m_tp(tp){m_priority = pri;};
  virtual ~PoolTask() = 0;
  virtual void operator()() = 0;
protected:
  ThreadPool& m_tp;
};

class DeleteTask:public PoolTask
{
public:
DeleteTask(ThreadPool& tp):PoolTask(tp ,5){};
~DeleteTask(){};
void operator()();

};

class PauseTask:public PoolTask
{
public:
  PauseTask(ThreadPool& tp):PoolTask(tp){};
  ~PauseTask(){};
  void operator()();
};

}//namespace ilrd
#endif
//ThreadPool& tp, enum priority
