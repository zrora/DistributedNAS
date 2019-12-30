#include <iostream>
#include <linux/nbd.h>  //nbd
#include "../dirmonitor/dirmonitor.hpp"
#include "../../cpp/DesignPatterns/Factory/factory.hpp"
#include "../dlloader/dlloader.hpp"
#include "../dlloader/sharedobject.hpp"
#include "../../cpp/DesignPatterns/Singletone/singletonesharedlibrary/singletone.hpp"
#include "../requesthandler.hpp"

// ilrd::RequestHandler* CreateReadHandler(Params& par)
// {
//   return new ReadHandler(par.bdp, par.mem);
// }
using namespace ilrd;
NBDRequestEngine_t::Command* CreateReadHandler(NBDRequestEngine_t::CommandArgs& par)
{
  return new ReadHandler(par.m_device, par.m_mem);
}

__attribute__((constructor))
static void NewRead()
{
  std::cout << "Adding Read handler to Factory" << '\n';
  Singleton<Factory<NBDRequestEngine_t::Command, int, RequestEngine<Request, NBDProxy,Memory>::CommandArgs> >::instance()->Add(NBD_CMD_READ, CreateReadHandler);

  // Factory<RequestHandler, int, Params> *fp;
  // fp = Singleton<Factory<RequestHandler, int, Params> >::instance();
  // fp->Add(NBD_CMD_READ, CreateReadHandler); // is added with external file (tets porposes)
  // std::cout << "READ: factory address" << fp<<'\n';

}
