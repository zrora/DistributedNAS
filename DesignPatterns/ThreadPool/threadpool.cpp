#include "threadpool.hpp"
namespace ilrd
{

  /***************************
  *****Compare FUNCTOR**********
  ****************************/
  bool CompareFunctor::operator()(boost::shared_ptr<TaskBase> lhs, boost::shared_ptr<TaskBase> rhs)
  {
    return (lhs->GetPriority() > rhs->GetPriority());
  }

  /****************************
  ***********THREADPOOL********
  ****************************/
ThreadPool::ThreadPool(int num_of_threads):m_is_paused(false), m_num_of_threads(num_of_threads)
{
  for (int i = 0; i < m_num_of_threads; i++)
  {
    CreateThread();
  }
}

void ThreadPool::SetThreadNumber(int new_num_of_threads)
{
  if (m_num_of_threads < new_num_of_threads)
  {
    int diff = new_num_of_threads - m_num_of_threads;
    if(m_is_paused)
    {
      for (int i = 0; i < diff; i++)
      {
        boost::shared_ptr<TaskBase> pausetask (new PauseTask(*this));
        AddTask(pausetask);
      }
    }
    for (int i = 0; i < diff; i++)
    {
      CreateThread();
      ++m_num_of_threads;
    }
  }

  else
  {
    int diff = m_num_of_threads - new_num_of_threads;
    for (int i = 0; i < diff; i++)
    {
      boost::shared_ptr<TaskBase>delete_task(new DeleteTask(*this));
      AddTask(delete_task);
      --m_num_of_threads;
    }
  }

}

void ThreadFunctor::RegisterFunctor(ThreadPool* tp)
{
  boost::unique_lock<boost::mutex> ul(tp->m_mutex);
  tp->m_map.insert(pair<boost::thread::id, ThreadFunctor*>(boost::this_thread::get_id(), this));
}


void ThreadPool::CreateThread()
{
  ThreadFunctor tf;
  boost::thread temp_thread(tf,this);
}

void ThreadPool::AddTask(boost::shared_ptr<TaskBase> tb)
{
  //waitable queue is thread safe
  m_wq.Enqueue(tb);
}

void ThreadPool::Pause()
{
  m_is_paused = true;
  for (int i = 0; i < m_num_of_threads; i++)
  {
    boost::shared_ptr<TaskBase> pausetask (new PauseTask(*this));
    AddTask(pausetask);
  }
}

void ThreadPool::Stop(boost::chrono::milliseconds timeinterval)
{
  boost::chrono::steady_clock::time_point timep (boost::chrono::steady_clock::now() + timeinterval);
  //unique lock == scopelock
  boost::unique_lock<boost::mutex> ul(m_mutex);
  //all paused tasks will continue
  m_cv.notify_all();
  for (int i = 0; i < m_num_of_threads; i++)
  {
    boost::shared_ptr<TaskBase>delete_task(new DeleteTask(*this));
    AddTask(delete_task);
  }

  while((m_num_of_threads) && boost::cv_status::timeout != m_stop_cv.wait_until(ul, timep)){};

  std::cout << "all threads are dead" << '\n';
}

void ThreadPool::LockCV()
{
  boost::unique_lock<boost::mutex> ul(m_mutex);
  while(m_is_paused)
  {
    m_cv.wait(ul);
  }
}

void ThreadPool::Resume()
{
  m_is_paused = false;
  m_cv.notify_all();
}
/***************************
*****THREAD FUNCTOR**********
****************************/
ThreadFunctor::ThreadFunctor():i_am_alive(true)
{}

void ThreadFunctor::operator()(ThreadPool* tp)
{
    RegisterFunctor(tp);
    while(i_am_alive)
    {
      std::cout << "waiting for a task" << '\n';
      boost::shared_ptr<TaskBase> tb = tp->m_wq.Dequeue();
      std::cout <<"thread id :"<< boost::this_thread::get_id()<<" is executing a task"<< '\n';
      (*tb)();
    }
}

void ThreadFunctor::KillMe()
{
  std::cout << "killing thread : " <<boost::this_thread::get_id()<<'\n';
  i_am_alive = false;
}
/****************************
****TASK BASE AND DERIVED****
****************************/
TaskBase::TaskBase(enum priority p):m_priority(p)
{}

TaskBase::~TaskBase(){}

int TaskBase::GetPriority()
{
  return m_priority;
}

PoolTask::~PoolTask()
{}

void PauseTask::operator()()
{
  m_tp.LockCV();
}

void DeleteTask::operator()()
{
  boost::unique_lock<boost::mutex> ul(m_tp.m_mutex);
  std::cout << "stop task" << '\n'<<'\n';

  std::map<boost::thread::id, ThreadFunctor*>::iterator it = m_tp.m_map.find(boost::this_thread::get_id());

  it->second->KillMe();

  m_tp.m_map.erase(it);

  --m_tp.m_num_of_threads;

  m_tp.m_stop_cv.notify_one();
}

}//namespace ilrd
