#include <boost/thread/thread.hpp>
#include "dirmonitor.hpp"

using namespace ilrd;
DirMonitor::DirMonitor(const std::string& path, uint32_t flags):m_path(path)
{
    m_fd = inotify_init();
    const char* temp = m_path.c_str();
    m_wd = inotify_add_watch(m_fd, temp, flags);
}

DirMonitor::~DirMonitor()
{
    inotify_rm_watch(m_fd,m_wd);
    close(m_fd);
}

void DirMonitor::Functor::operator()(int fd, Dispatcher<std::string>* disp )
{
  const int event_size =  sizeof (struct inotify_event) ;
  const int buf_len =  1024 * ( event_size + 16  );
  char buffer[buf_len];

  while(read(fd, buffer, buf_len))
  {
    struct inotify_event *event = (struct inotify_event*) &buffer;
    std::string name = event->name;
    if(event->len)
    {
        std::cout << "file changed" << '\n';
        disp->NotifyAll(event->name);
    }
  }
}

void DirMonitor::Monitor()
{
  m_monitoring = true;
  Functor func;
  boost::thread(func, m_fd, &m_disp);
}

void DirMonitor::StopMonitoring()
{
  m_monitoring = false;
}


void DirMonitor::subscribe(CallbackBase<std::string>& cb_dir)
{
  m_disp.Attach(&cb_dir);
}

void DirMonitor::unsubscribe(CallbackBase<std::string>& cb_dir)
{
  m_disp.Detach(&cb_dir);
}

std::string DirMonitor::GetPath()
{
  return m_path;
}
