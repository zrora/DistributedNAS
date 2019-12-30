#include "requesthandler.hpp"
namespace ilrd
{
  RequestHandler::RequestHandler(NBDProxy& bdp, Memory& mem):m_bdproxy(bdp), m_mem(mem)
  {}

  ReadHandler::ReadHandler(NBDProxy& bdp, Memory& mem):RequestHandler(bdp, mem)
  {}

  void ReadHandler::operator()(Request& rh)
  {
      std::cout << "request handler function (read handler)" << '\n';
      m_mem.read(rh);
      std::cout << this<< '\n';
      m_bdproxy.Done(rh);
  }

  WriteHandler::WriteHandler(NBDProxy& bdp, Memory& mem):RequestHandler(bdp, mem)
  {}

  void WriteHandler::operator()(Request& rh)
  {
      std::cout << "request handler function (write handler)" << '\n';
      m_mem.write(rh);
      m_bdproxy.Done(rh);
  }

  DiscHandler::DiscHandler(NBDProxy& bdp, Memory& mem):RequestHandler(bdp, mem)
  {}

  void DiscHandler::operator()(Request& rh)
  {
    std::cout << "do somthing to disconnect" << '\n';
    //need to call memory disc??
    std::cout << rh.m_type << '\n';
  }

}//namespace ilrd
