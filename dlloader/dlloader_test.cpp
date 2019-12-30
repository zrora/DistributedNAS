#include "dlloader.hpp"
#include "sharedobject.hpp"
#include "../dirmonitor/dirmonitor.hpp"
using namespace ilrd;

int main()
{
  DirMonitor dm("/home/bob45/svn/Proj/src/project/dlloader",  IN_CLOSE_WRITE | IN_MOVED_TO) ;
  DLLoader dll(&dm);
  // DirMonitor.subscribe(dm);
  dm.Monitor();

  while(1);

  return 0;
}
