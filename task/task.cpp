// #include "task.hpp"
// #include "../../cpp/DesignPatterns/Factory/factory.hpp"
// #include "../../cpp/DesignPatterns/Singletone/singletonesharedlibrary/singletone.hpp"
// #include "../requesthandler.hpp"
//
// namespace ilrd
// {
//
// Task::Task(Request& req,  RequestEngine<Request, BDProxy<void(*)(Request&)>, Memory>::CommandArgs& par , enum priority p):TaskBase(p), m_request(req),m_par(par)
// { }
//
// Task::~Task(){}
//
// void Task::operator()()
// {
//   RequestHandler* rh;
//   rh = Singleton<Factory<RequestHandler, int, RequestEngine<Request, BDProxy<void(*)(Request&)>, Memory>::CommandArgs> >::instance()->Create(m_request.m_type, m_par);
//   (*rh)(m_request);
//   std::cout << "Task: task ran from queue by thread" << '\n';
// }
// }//namespace ilrd
