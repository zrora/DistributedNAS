#ifndef __SINGLETON_SCHED
#define __SINGLETON_SCHED
/******************************************************************
this file must be defined by user for each different Request engine
*******************************************************************/
#include "../../cpp/DesignPatterns/Singletone/singletonesharedlibrary/singletone.hpp"
#include "../schedualer/schedualer.hpp"

namespace ilrd
{
    CREATE_INSTANCE(Schedualer)
}//namespace ilrd

#endif
