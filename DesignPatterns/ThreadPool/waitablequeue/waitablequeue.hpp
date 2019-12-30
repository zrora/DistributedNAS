#include <functional>
#include <queue>
#include <vector>
#include <iostream>
#include <boost/chrono/duration.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
using namespace std;

template <class T, class Container, class Compare>
class WaitableQueue
{
public:
  bool Enqueue(T& data);
  //block forever dequeue , when returning data got inside the queue
  T Dequeue();
  bool Dequeue(boost::chrono::duration<long ,boost::milli>, T* return_value);
  size_t Size() const ;
  bool IsEmpty() const;

private:
   std::priority_queue<T, Container, Compare> m_pq;
   //insure thread safety with a conditional variable
   boost::mutex m_mutex;
   boost::condition_variable m_cv;
};


template <class T, class Container, class Compare>
size_t WaitableQueue<T,Container,Compare>::Size()const
{
  return m_pq.size();
}

template <class T, class Container, class Compare>
bool WaitableQueue<T,Container,Compare>::IsEmpty()const
{
  return m_pq.empty();
}

template <class T, class Container, class Compare>
bool WaitableQueue<T,Container,Compare>::Enqueue(T& data)
{
  boost::unique_lock<boost::mutex> ul(m_mutex);
  m_pq.push(data);
  // notify one condition_variable
  m_cv.notify_one();
  return true;
}

template <class T, class Container, class Compare>
T WaitableQueue<T, Container, Compare>::Dequeue()
{
  //if the queue is empty block the thread;
  boost::unique_lock<boost::mutex> ul(m_mutex); //lock mutex
  while (IsEmpty())//while because the cv can wake up spouriosly!!
  {
    //mutex is unlocked when cv is called!
    //m_cv gonna release threads one by one
    m_cv.wait(ul);
    //mutex is locked when cv is released
  }
  T return_value = m_pq.top();
  m_pq.pop();

  return return_value;
}

template <class T, class Container, class Compare>
bool WaitableQueue<T, Container, Compare>::Dequeue(boost::chrono::duration<long ,boost::milli> duration, T* return_value)
{
//lock mutex until timeout or signal from writer!!
  boost::unique_lock<boost::mutex> ul(m_mutex);
  while (IsEmpty())
  {
    if (m_cv.wait_for(ul, duration) == boost::cv_status::timeout)
    {
      return false;
    }
  }
  *return_value = m_pq.top();
  m_pq.pop();
  return true;
}
