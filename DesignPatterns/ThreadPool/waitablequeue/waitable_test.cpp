#include <pthread.h>
#include "waitablequeue.hpp"

int array1[] = {1,2,3,4,5,6,7,8,9,10};
int array2[] = {11,12,13,14,15,16,17,18,19,20};
int array3[] = {21, 22,23,24,25,26,27,28,29,30};

void enqueue_thread_func1 (WaitableQueue<int, std::vector<int>, std::greater<int> >* wq);
void enqueue_thread_func2 (WaitableQueue<int, std::vector<int>, std::greater<int> > *wq);
void enqueue_thread_func3 (WaitableQueue<int, std::vector<int>, std::greater<int> > *wq);

void dequeue_thread_func1 (WaitableQueue<int, std::vector<int>, std::greater<int> > *wq);
void dequeue_thread_func2 (WaitableQueue<int, std::vector<int>, std::greater<int> > *wq);

int main(int argc, char const *argv[])
{
  WaitableQueue<int, std::vector<int>, std::greater<int> > wq;
std::cout << "running threads" << '\n';
  boost::thread enqueue_thread1 (enqueue_thread_func1, &wq);
  boost::thread enqueue_thread2 (enqueue_thread_func2, &wq);
  boost::thread enqueue_thread3 (enqueue_thread_func3, &wq);

  boost::thread dequeue_thread2(dequeue_thread_func2, &wq);
  boost::thread dequeue_thread1(dequeue_thread_func1, &wq);
  sleep(2);
  return 0;
}

void enqueue_thread_func1 (WaitableQueue<int, std::vector<int>, std::greater<int> >* wq)
{
  // usleep(1000);
  std::cout << "enq 1" << '\n';
  for (size_t i = 0; i < 10; i++)
  {
    wq->Enqueue(array1[i]);
  }

}

void enqueue_thread_func2 (WaitableQueue<int, std::vector<int>, std::greater<int> >* wq)
{
  // usleep(1000);
  std::cout << "enq 2" << '\n';
  for (size_t i = 0; i < 10; i++)
  {
    wq->Enqueue(array2[i]);
  }

}
void enqueue_thread_func3 (WaitableQueue<int, std::vector<int>, std::greater<int> >* wq)
{
  // usleep(1000);
  std::cout << "enq 3" << '\n';
  for (size_t i = 0; i < 10; i++)
  {
    wq->Enqueue(array3[i]);
  }

}

void dequeue_thread_func1 (WaitableQueue<int, std::vector<int>, std::greater<int> >* wq)
{
  std::cout << "dec 1" << '\n';
  for (size_t i = 0; i < 20; i++)
  {
    usleep(1000);
    std::cout << wq->Dequeue() << '\n';

  }
}

void dequeue_thread_func2 (WaitableQueue<int, std::vector<int>, std::greater<int> >* wq)
{
  std::cout << "dec 2" << '\n';
  int val = 0;
  boost::chrono::milliseconds mils(1);

  for (size_t i = 0; i < 10; i++)
  {
    wq->Dequeue(mils, &val);
    std::cout <<val << '\n';
  }
}
