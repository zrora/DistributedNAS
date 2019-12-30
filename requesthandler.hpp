#ifndef __REQUESTHANDLER
#define __REQUESTHANDLER
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include "bdproxy.hpp"
#include "memory.hpp"
#include "./framework/requestengine.hpp"
#include "nbdproxy.hpp"
// using namespace ilrd;
namespace ilrd
{

typedef RequestEngine<Request, NBDProxy, Memory> NBDRequestEngine_t;

class RequestHandler:public NBDRequestEngine_t::Command
{
public:
  RequestHandler(NBDProxy&, Memory&);
  virtual void operator()(Request&) = 0;

protected:
  NBDProxy& m_bdproxy;
  Memory& m_mem;
};


class ReadHandler:public RequestHandler
{
public:
  ReadHandler(NBDProxy& bdp, Memory& mem);
  void operator()(Request&);
};

class WriteHandler:public RequestHandler
{
public:
  WriteHandler(NBDProxy& bdp, Memory& mem);
  void operator()(Request&);
};

class DiscHandler:public RequestHandler
{
public:
  DiscHandler(NBDProxy& bdp, Memory& mem);
  void operator()(Request&);
};


}//namespace ilrd
#endif
