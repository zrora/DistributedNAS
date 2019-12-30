#include "threadpool.hpp"
using namespace ilrd;

class Task:public TaskBase
{
public:
Task(enum priority);
~Task();
virtual void operator()();

};
Task::Task(enum priority p):TaskBase(p)
{ }

Task::~Task(){}

void Task::operator()()
{
  std::cout <<GetPriority()<<" priority task"<< '\n'<<'\n';
}

 int main()
 {
   boost::shared_ptr<TaskBase> low_task(new Task(LOW));
   boost::shared_ptr<TaskBase> med_task(new Task(MEDIUM));
   boost::shared_ptr<TaskBase> high_task(new Task(HIGH));

   ThreadPool tp(120);

   std::cout << "now pausing to add tasks" << '\n'<<'\n'<<'\n'<<'\n';
   tp.Pause();
   std::cout << "Adding tasks to queue" << '\n';
   // sleep(2);

   for (size_t i = 0; i < 40; i++)
   {
    tp.AddTask(low_task);
    tp.AddTask(med_task);
    tp.AddTask(high_task);
   }
   std::cout << "tasks added" << '\n';
   // sleep(2);
   std::cout << "resuming in 2 sec" << '\n'<<'\n'<<'\n'<<'\n';
   tp.Resume();
   std::cout << "killing all threads" << '\n';
   // sleep(2);
   boost::chrono::milliseconds ms(1000);
   tp.Stop(ms);

   return 0;
 }
