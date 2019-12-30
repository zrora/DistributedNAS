#ifndef __NBDPROXY
#define __NBDPROXY

#include <boost/thread/mutex.hpp>
#include <boost/function.hpp>
// #include "request.hpp"
#include "bdproxy.hpp"
namespace ilrd
{
//NDBDiscReq has the same members as Request (m_type)
typedef Request NDBDiscReq;
int read_all(int fd, char* buf, size_t count);
int write_all(int fd, char* buf, size_t count);


class NBDTranslator
{
public:
    u_int16_t ToNetWork(u_int16_t);
    u_int32_t ToNetWork(u_int32_t);
    u_int64_t ToNetWork(u_int64_t);

    u_int16_t ToHost(u_int16_t);
    u_int32_t ToHost(u_int32_t);
    u_int64_t ToHost(u_int64_t);

private:
    static int s_endianess;
};


class NBDProxy:public BDProxyFunction,private boost::noncopyable
{
public:
    NBDProxy(u_int64_t size, const char* dev_file);
    ~NBDProxy();
    bool IsValid();
    void Done(struct Request &req);
    void OnSocketReady();

    int GetFd();

private:
    static void* RunServer(void* ptr);
    boost::mutex m_mutex;
    int m_socket_fd[2];
    int m_nbd_fd;
};


//request also contains int m_type;
struct NBDRWReq:public Request
{
    NBDRWReq(struct nbd_request, char* data = 0);
    ~NBDRWReq();
    int m_size;
    int m_offset;
    char m_handle[8];
    char* m_data;
};

}//namespace ilrd
#endif
