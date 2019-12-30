#include <boost/atomic.hpp>
#include <boost/thread/mutex.hpp>
#include <stdlib.h>


namespace ilrd
{
using boost::atomic;

template<typename T>
class Singleton
{
public:
  static T* instance();
  static void KillInstance(void);
private:
  //constructor && destructor are generated
  static boost::atomic<T*> m_instance;
  static boost::mutex m_mutex;
};

// void kill (void){}

template<typename T>
boost::mutex Singleton<T>::m_mutex;


template<typename T>
atomic<T*> Singleton<T>::m_instance(0);

template<typename T>
void Singleton<T>:: KillInstance()
{
  //thing that are writter after me will not be before me
  T* temp = m_instance.load(boost::memory_order_consume);

  if (!temp)
  {
    boost::mutex::scoped_lock lock(m_mutex);
    if (!temp)
    {
      delete temp ;
    }
  }
}

template<typename T>
T* Singleton<T>:: instance()
{
  T* temp = m_instance.load(boost::memory_order_consume);//thing that are writter after me will not be before me

  if (!temp)
  {
     boost::mutex::scoped_lock lock(m_mutex);
     if (!temp)
     {
       temp = new T;
       m_instance.store(temp, boost::memory_order_release);
       atexit(KillInstance);
     }

  }

  return temp;
}




}//namespace ilrd
