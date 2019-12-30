#include "dlloader.hpp"
namespace ilrd
{
DLLoader::DLLoader(DirMonitor* monitor):
      m_dm(monitor),m_cb(*this, &DLLoader::Load, &DLLoader::OnMonitorDeath)
{
  m_dm->subscribe(m_cb);
}

DLLoader::~DLLoader()
{
  // kill all instances inside vector
  std::vector<SharedObject*>::iterator it = m_so.begin();
  for(;it < m_so.end(); it++)
  {
    delete (*it);
    m_so.erase(it);
  }
}

void DLLoader::Load(std::string event)
{
  std::string temp =  m_dm->GetPath()+'/'+event;
  std::cout <<"Dlloading: "<< temp << '\n';
  m_so.push_back(new SharedObject(temp));
}

void DLLoader::OnMonitorDeath()
{}

void DLLoader::SetMonitor(DirMonitor* monitor)
{
  m_dm = monitor;
}

}//namespace ilrd
