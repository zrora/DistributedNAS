#ifndef __MASTERPROXY
#define __MASTERPROXY

#include <boost/thread/mutex.hpp>
#include "bdproxy.hpp"
#include <string>
#include <netdb.h>  //address structs
#include <string.h> //memset
#include <unistd.h> //close

#include <boost/function.hpp>
// class NBDProxy:public BDProxy<boost::function<void (Request&, NBDProxy&)> >,private boost::noncopyable
namespace ilrd
{
//slaveproxy must change to bdproxy
class MasterProxy:public BDProxyFunction,private boost::noncopyable
{
public:
    MasterProxy(u_int64_t size, std::string port);
    ~MasterProxy();

    void OnSocketReady();
    void Done(struct Request &req);

    int GetFd();

private:
    int m_socket;
    struct sockaddr_in m_clientaddr; /* client addr */
    addrinfo *m_res;
};
//master proxy act as a udp server and located on each one of the slaves

// struct MasterRWReq:public Request
// {
//     MasterRWReq(struct Request, char* data = 0);
//     ~MasterRWReq();
//     int m_size;
//     int m_offset;
//     char m_handle[8];
//     char* m_data;
// };



MasterProxy::MasterProxy(u_int64_t size, std::string port):BDProxyFunction(size)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;        //use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_DGRAM;     //udp stream socket
    hints.ai_flags = AI_PASSIVE;        //fill in my IP for me

    getaddrinfo(NULL, port.c_str(), &hints, &m_res);
    m_socket = socket(m_res->ai_family, m_res->ai_socktype, m_res->ai_protocol);
    bind(m_socket, m_res->ai_addr, m_res->ai_addrlen);
}

MasterProxy::~MasterProxy()
{
    freeaddrinfo(m_res);
    close(m_socket);
}

int MasterProxy::GetFd()
{
    return m_socket;
}

//reads data from slave
void MasterProxy::OnSocketReady()
{
    //read from socket on slave to request
    //push the request to thread pool
    //the slave handler will take care of this type of task
    // the handler write to file and calls done
    //read from socket to a NBDRWReq(with changed type!)
    //push the request to thread pool -> (changes the type to network read/write)
    //the network handler will take care of this type of task
    std::cout << "on socket ready" << '\n';
    struct NBDRWReq request;
    read_all(m_socket, (char*)&request, sizeof(NBDRWReq));
  // read(m_socket_fd[0], &request, sizeof(nbd_request));

    if(ntohl(request.type) == NBD_CMD_WRITE)
    {
      //ex:nbd asked to read-> buffer was filled on the slave side->
      // request returns with filled buffer

      // slave must change the type since the handler is gonna be different
      // type = factory key
      // NBD_CMD_READ_R = filled buffer
      // NBD_CMD_WRITE_R = the data was written on

      char *temp = new char[ntohl(request.len)];
      read_all(m_socket, temp, ntohl(request.len));
      request.m_data = temp;
    }

    std::cout << "nbd_proxy: calling m_cb(my_nbd_request, *this)" << '\n';
    (m_cb)(*request, *this);


}

//after data was sent
void MasterProxy::Done(struct Request &req)
{
    //writes the reply socket
}

}//namespaceilrd


#endif
