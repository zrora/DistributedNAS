#ifndef __REQUEST_ENGINE_HPP__
#define __REQUEST_ENGINE_HPP__

#include "../dirmonitor/dirmonitor.hpp"
#include "../dlloader/dlloader.hpp"
#include "../../cpp/DesignPatterns/Factory/factory.hpp"
#include "../../cpp/DesignPatterns/Singletone/singletonesharedlibrary/singletone.hpp"
#include "../../cpp/DesignPatterns/ThreadPool/threadpool.hpp"
#include "../bdproxy.hpp"
// #include "../memory.hpp"



namespace ilrd
{
  /*************************** RequestEngine ***********************************/

  // Request: a template parameter that represents data type for the engine to work on.
  //
  // Device: a class template parameter,
  // which will be connected to the RequestEngine for a given fd.
  // Device must implement three methods: OnFDReady, Acknowledge/Done, and SetRequestCB.
  //
  // Memory: a class template parameter which is in charge of managing R/W operations

template<class Request, class Device, class Memory>
class RequestEngine
{
private:
    class RETask;
public:
  class CommandArgs;
  //Command is the father of requesthandler when using device = nbdproxy
  class Command;

  typedef Command*(Handler_t)(CommandArgs&);
  typedef Factory<Command, int, CommandArgs> ReqFactory_t;

  RequestEngine(Memory& mem, int threads, const std::string dm_path = "");
  // ~RequestEngine();

  bool AddToFactory(int factory_key, Handler_t handler_func );
  bool AddResource(int fd, Device* device);
  void Run();

private:

  friend class RETask;
  Memory& m_mem;
  std::map<int, Device*> m_map;
  bool m_is_valid;

  DirMonitor* m_dm;
  DLLoader* m_dll;
  ThreadPool m_tp;
  void RequestCB(Request& req, Device &device);
};

/***************************
********RETask*************
*************************/
template <typename Request, class Device, class Memory>
class RequestEngine<Request, Device, Memory>::
RETask:public TaskBase
{
public:
  RETask(Request& req, Device& device,Memory& mem, enum priority = LOW);
  // ~RETask();
  virtual void operator()();

private:
  Request& m_request;
  Device& m_dev;
  Memory& m_mem;
};

template <typename Request, class Device, class Memory>
RequestEngine<Request, Device, Memory>::
RETask::RETask(Request& req, Device& device, Memory& mem, enum priority p):
                                                              TaskBase(p),
                                                              m_request(req),
                                                              m_dev(device),
                                                              m_mem(mem)


{}

// template <typename Request, class Device, class Memory>
// RequestEngine<Request, Device, Memory>::
// RETask::~RETask()
// {}

template <typename Request, class Device, class Memory>
void RequestEngine<Request, Device, Memory>::
RETask::operator()()
{
  Command* rh;
  CommandArgs ca(m_dev, m_mem);
  rh = Singleton<ReqFactory_t>::instance()->Create(m_request.m_type, ca);

  (*rh)(m_request);
}

/***************************
********RequestCB***********
***************************/
template <typename Request, class Device, class Memory>
void RequestEngine<Request,Device,Memory>::
RequestCB(Request& req, Device &device)
{
  // CommandArgs ca(req, device);
  std::cout << "RequestCB:: adding task to m_tp" << '\n';
  boost::shared_ptr<TaskBase> task(new RETask(req, device, m_mem));
  m_tp.AddTask(task);
}



/***************************
********RequestEngine*******
***************************/

template<class Request, class Device, class Memory>
RequestEngine<Request, Device, Memory>::
RequestEngine(Memory& mem, int threads ,const std::string dm_path):
                                                                  m_mem(mem),
                                                                  m_is_valid(true),
                                                                  m_dm(0),
                                                                  m_dll(0),
                                                                  m_tp(threads)
{
  if(dm_path != "")
  {
    m_dm = new DirMonitor(dm_path, IN_MOVED_TO);
    m_dll = new DLLoader (m_dm);
    m_dm->Monitor();
  }
}

template<class Request, class Device, class Memory>
bool RequestEngine<Request,Device,Memory>::
AddToFactory(int factory_key, Handler_t handler_func)
{
  return Singleton<ReqFactory_t>::instance()->Add(factory_key, handler_func);
}

template<class Request, class Device, class Memory>
bool RequestEngine<Request,Device,Memory>::
AddResource(int fd, Device *device)
{

  device->SetCallBack(boost::bind(&RequestEngine::RequestCB, this, _1, _2));
  m_map.insert(std::pair<int, Device*>(fd, device) );

  return true;
}

template<class Request, class Device, class Memory>
void RequestEngine<Request,Device,Memory>::
Run()
{
  int biggest_fd = 0 ;
  fd_set master;
  FD_ZERO(&master);
  typename std::map<int, Device*>::iterator it = m_map.begin();
  for(; it != m_map.end(); ++it)
  {
    FD_SET(it->first, &master);
    std::cout << "RUN: fd added to set is: " <<it->first<< '\n';
    biggest_fd = it->first + 1;
  }
  // --it;

  std::cout << "RUN: biggest_fd = " << biggest_fd<< '\n';

  while(m_is_valid)
  {
    select(biggest_fd, &master, NULL, NULL, NULL);
    for(it = m_map.begin(); it != m_map.end(); ++it)
    {
      if(FD_ISSET(it->first, &master))
      {
        it->second->OnSocketReady();
      }
    }
  }
}

/***************************
********CommandArgs********
*************************/
template <class Request, class Device, class Memory>
class RequestEngine<Request, Device, Memory>::CommandArgs
{
public:
  CommandArgs(Device&, Memory&);
  Device& m_device;
  Memory& m_mem;
};

template<class Request, class Device, class Memory>
RequestEngine<Request, Device, Memory>::
CommandArgs::CommandArgs(Device& dev, Memory& mem):
                                                m_device(dev),
                                                m_mem(mem)
{}
/***************************
********Command*************
****************************/
template <typename Request, class Device, class Memory>
class RequestEngine<Request, Device, Memory>::Command
{
public:
  virtual void operator()(Request& req) = 0;
};

} // end of namespace ilrd
#endif
