
#ifndef __BDPROXY
#define __BDPROXY

#include <sys/types.h>
#include "boost/function.hpp"
#include "request.hpp"

// #include "callback.hpp"

namespace ilrd
{

// class CallBack;
// typedef void(*call_back_t)(Request& request);
// template<class Callback>
template<typename Callback>
class BDProxy
{
  // friend class Task;
  // typedef void (*CallbackFunc)(Request&);
public:
    explicit BDProxy(u_int64_t size );
    virtual ~BDProxy();

    void SetCallBack(Callback cb);
    virtual void Done(struct Request &req)= 0;
    virtual void OnSocketReady() = 0;

protected:
  Callback m_cb;//void(*)(Request&, NBDProxy&)
  u_int64_t m_size;
};


template<class Callback>
BDProxy<Callback>::~BDProxy()
{}

template<class Callback>
BDProxy<Callback>::BDProxy(u_int64_t size):
                                        m_size(size)
{}

template<class Callback>
void BDProxy<Callback>::SetCallBack(Callback cb)
{
  m_cb = cb;
}

class BDProxyFunction:public BDProxy <boost::function<void (Request&, BDProxyFunction&)> >
{
public:
    BDProxyFunction(u_int64_t size):BDProxy(size)
    {}
};

}//namespace ilrd
#endif
