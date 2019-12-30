/*********************************************************************
NBD PROXY TEST, MAKE SURE TO COMPILE .SO FILES BEFORE RUNNING THE APP
*********************************************************************/

#include <iostream>     // cout
#include <stdlib.h>     //  exit()
#include <arpa/inet.h>  // htonl
#include <fcntl.h>      //open
#include <unistd.h>
#include <sys/types.h>
#include <linux/nbd.h>  //nbd
/*select*/
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>
#include "requesthandler.hpp"
#include "bdproxy.hpp"
#include "nbdproxy.hpp"
#include "request.hpp"
#include "utils.hpp"
#include "./framework/requestengine.hpp"
#include "requesthandler.hpp"

using namespace std;
using namespace ilrd;

/******************request handler factory functions**********************/
NBDRequestEngine_t::Command* CreateReadHandler(NBDRequestEngine_t::CommandArgs& par)
{
  return new ReadHandler(par.m_device, par.m_mem);
}

NBDRequestEngine_t::Command* CreateWriterHandler(NBDRequestEngine_t::CommandArgs& par)
{
 return new WriteHandler(par.m_device, par.m_mem);
}

NBDRequestEngine_t::Command* CreateDiscHandler(NBDRequestEngine_t::CommandArgs& par)
{
 return new DiscHandler(par.m_device, par.m_mem);
}

//global nbdproxy pointer
NBDProxy *nbd_ptr(0);
int file_fd(0);

inline void RunTest(int (*test_func)(void))
{
    int result = test_func();
    if (0 == result)
    {
        cout << "[\033[0;92mOK\033[0m]" << endl;
    }
    else
    {
        cout << "[\033[0;91mFAIL!\033[0m]" << endl;
        exit(result);
    }
}

int Flow1(void);

int main()
{
    RunTest(&Flow1);
    std::cout << "test over" << '\n';
    return 0;
}

int Flow1(void)
{
  // set size to 128 Mb
    u_int64_t size = 0x1<<27;

    file_fd = open("file.r.w", O_RDWR);
    ThrowIfBad<Error>(file_fd == -1, "file wasnt open");
    ThrowIfBad<Error>(ftruncate(file_fd, size) == -1, "ftruncate failed");

    NBDProxy nbd_inst(size, "/dev/nbd3");
    MemoryFile mf(file_fd);


    RequestEngine<Request, NBDProxy, Memory> re (mf, 6, "/home/bob45/svn/Proj/src/project/pnpcommands");
    re.AddResource(nbd_inst.GetFd(), &nbd_inst);
    std::cout << "Adding to facotry " << '\n';
    re.AddToFactory(NBD_CMD_WRITE, CreateWriterHandler);
    re.AddToFactory(NBD_CMD_DISC, CreateDiscHandler);
    re.AddToFactory(NBD_CMD_READ, CreateReadHandler);
    std::cout << "waiting " << '\n';
    re.Run();


    return 0;
}
