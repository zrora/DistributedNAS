/*#ifndef __TASK
#define __TASK

#include "../nbdproxy.hpp"
#include "../framework/requestengine.hpp"
#include"../../cpp/DesignPatterns/ThreadPool/threadpool.hpp"
#include "../memory.hpp"
namespace ilrd
{

class Task:public TaskBase
{

public:
  Task(Request& req, RequestEngine<Request, NBDProxy, Memory>::CommandArgs& par, enum priority = LOW);
  ~Task();
  virtual void operator()();

private:
  Request& m_request;
  //CommandArgs contains the bd proxy ref and the mem ref
  RequestEngine<Request, NBDProxy, Memory>::CommandArgs& m_par;
};

}//namespace ilrd

#endif*/
