// #include "callback.hpp"
// #include "memory.hpp"
// #include "bdproxy.hpp"
// #include "requesthandler.hpp"
//
// #include "task/task.hpp"
// namespace ilrd
// {
//
// CallBack::CallBack(CommandArgs par):m_param(par),m_tp(4)
// {}
//
//
// void CallBack::operator()(Request& req)
// {
//   // the request handler class will deal with the handler
//   boost::shared_ptr<TaskBase> task(new Task(req, m_param));
//   m_tp.AddTask(task);
//   std::cout << "CallBack: task added to queue" << '\n';
// }
//
// }//namespace ilrd
