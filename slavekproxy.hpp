//this proxy emulates a slave
//the slave sends Acknowledgement of recived data
//


#ifndef __SLAVEPROXY
#define __SLAVEPROXY

#include <boost/thread/mutex.hpp>
#include "bdproxy.hpp"
#include <boost/function.hpp>//boost::function
#include <string>//std::string
#include <netdb.h>//freeaddrinfo
#include <iostream>//cout

namespace ilrd
{

enum {
	NBD_CMD_READ = 0,
	NBD_CMD_WRITE = 1,
	NBD_CMD_DISC = 2,
    NBD_CMD_READ_R = 3,
    NBD_CMD_WRITE_R = 4
};

int read_all(int fd, char* buf, size_t count)
{
  std::cout << "read all" << '\n';
  int bytes_read;

  while (count > 0)
  {
    bytes_read = read(fd, buf, count);
    buf += bytes_read;
    count -= bytes_read;
  }

  std::cout << "done read all" << '\n';

  return 0;
}

int write_all(int fd, char* buf, size_t count)
{
  std::cout << "write all" << '\n';
  int bytes_written;

  while (count > 0) {
    bytes_written = write(fd, buf, count);
    buf += bytes_written;
    count -= bytes_written;
  }

  return 0;
}

class SlaveProxy:public BDProxyFunction,private boost::noncopyable
{
public:
    SlaveProxy(u_int64_t size, std::string ip, std::string port);
    ~SlaveProxy();

    void Done(struct Request &req);
    void OnSocketReady();

    int GetFd();
private:
    struct addrinfo *m_res;
    int m_udp_socket_fd;
};

//
// struct SlaveRWReq:public Request
// {
//     SlaveRWReq(struct Request, char* data = 0);
//     ~SlaveRWReq();
//     int m_size;
//     int m_offset;
//     char m_handle[8];
//     char* m_data;
// };

//slave proxy is one of masters sockets to read/write to from/to using UDP ai_protocol
//this proxy acts as the client while the proxy on the slave side acts as the server
SlaveProxy::SlaveProxy(u_int64_t size, std::string ip, std::string port): BDProxyFunction(size)
{
    //prepare socket
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_DGRAM;
    hints.ai_protocol=0;
    hints.ai_flags=AI_ADDRCONFIG;

    getaddrinfo(ip.c_str(), port.c_str(), &hints, &m_res);
    m_udp_socket_fd = socket(m_res->ai_family, m_res->ai_socktype, m_res->ai_protocol);
}

SlaveProxy::~SlaveProxy()
{
    freeaddrinfo(m_res);
    close(m_udp_socket_fd);
}

void SlaveProxy::OnSocketReady()
{
//read from socket to a NBDRWReq(with changed type!)
//push the request to thread pool -> (changes the type to network read/write)
//the network handler will take care of this type of task
  std::cout << "on socket ready" << '\n';
  struct NBDRWReq request;
  read_all(m_udp_socket_fd, (char*)&request, sizeof(NBDRWReq));
  // read(m_socket_fd[0], &request, sizeof(nbd_request));

if(ntohl(request.type) == NBD_CMD_READ_R)
  {
      //ex:nbd asked to read-> buffer was filled on the slave side->
      // request returns with filled buffer

      // slave must change the type since the handler is gonna be different
      // type = factory key
      // NBD_CMD_READ_R = filled buffer
      // NBD_CMD_WRITE_R = the data was written on

      char *temp = new char[ntohl(request.len)];
      read_all(m_udp_socket_fd, temp, ntohl(request.len));
      request.m_data = temp;
  }

  std::cout << "nbd_proxy: calling m_cb(my_nbd_request, *this)" << '\n';
  (m_cb)(*request, *this);

}

//after data was sent
void SlaveProxy::Done(struct Request &req)
{
    //search request in udp sent list and delete it
    //calls the nbdproxy done
    std::cout << "Slave Proxy request was done" << '\n';
}

}//namespaceilrd

#endif
