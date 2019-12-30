#ifndef __DIRMONITOR
#define __DIRMONITOR
#include<string>
#include <sys/inotify.h>
#include "../../cpp/DesignPatterns/observer/observer.hpp"
namespace ilrd
{

//use inotify
// enum event{FILE_ADDED};

class DirMonitor
{
private:
  class Functor
  {
  public:
    void operator()(int fd, Dispatcher<std::string>* disp);
  };
public:
  DirMonitor(const std::string& path, uint32_t flags);
  ~DirMonitor();

  std::string GetPath();
  void subscribe(CallbackBase<std::string>& cb_dir);
  void unsubscribe(CallbackBase<std::string>& cb_dir);
  void Monitor();
  void StopMonitoring();

private:
  Dispatcher<std::string> m_disp;
  std::string m_path;
  bool m_monitoring;
  int m_fd;
  int m_wd;

};

}//namespace ilrd
#endif
