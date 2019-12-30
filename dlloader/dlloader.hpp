#ifndef __DLLOADER
#define __DLLOADER

#include <vector>
#include "sharedobject.hpp"
#include "../dirmonitor/dirmonitor.hpp"

namespace ilrd
{

class DLLoader
{
public:
  DLLoader(DirMonitor* monitor = 0);
  ~DLLoader();

  void Load(std::string event);
  void OnMonitorDeath();
  void SetMonitor (DirMonitor* monitor);

private:
  std::vector<SharedObject*> m_so;
  DirMonitor* m_dm;
  Callback<std::string, DLLoader> m_cb;
};

}//namespace ilrd
#endif
