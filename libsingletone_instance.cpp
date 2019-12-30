#ifndef __SINGLETON_FAC
#define __SINGLETON_FAC
/******************************************************************
this file must be defined by user for each different Request engine
*******************************************************************/
#include "../cpp/DesignPatterns/Singletone/singletonesharedlibrary/singletone.hpp"
#include "../cpp/DesignPatterns/Factory/factory.hpp"
#include "requesthandler.hpp"

namespace ilrd
{
    // typedef Factory<RequestEngine<NBDRWReq, NBDProxy, Memory> >::Command*, int, RequestEngine<NBDRWReq, NBDProxy, Memory> >::CommandArgs> factory_t
    typedef Factory<NBDRequestEngine_t::Command, int, RequestEngine<Request, NBDProxy,Memory>::CommandArgs> factory_t;
    CREATE_INSTANCE(factory_t)
}//namespace ilrd

#endif
