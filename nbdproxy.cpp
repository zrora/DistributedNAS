#include <fcntl.h>      //open
#include <linux/nbd.h>  //ioctl
#include <netinet/in.h> //ntoh
#include <signal.h>     //signals
#include <sys/ioctl.h>  //ioctl
#include <unistd.h>     //close
#include <pthread.h>    //thread
#include <cstring>
#include <endian.h>
//my headers
#include "utils.hpp"
#include "bdproxy.hpp"
#include "request.hpp"
#include "nbdproxy.hpp"
#include "task/task.hpp"
#include "memory.hpp"
namespace ilrd
{
    //fwd decleration
    enum endian {LITTLEENDIAN,BIGENDIAN};
    int SetEndianess();
    //initialize static member of translator
    int NBDTranslator::s_endianess = SetEndianess();
    int SetEndianess()
    {
        unsigned int i = 0x1;
        char *c = (char*)&i;
        if(*c)
        {
            return LITTLEENDIAN;
        }
        return BIGENDIAN;
    }


    static int nbd_dev_to_disconnect = -1;
    static void disconnect_nbd(int signal)
    { //signal handler function
        (void)signal;
        if (nbd_dev_to_disconnect != -1)
        {
            ioctl(nbd_dev_to_disconnect, NBD_DISCONNECT);
            nbd_dev_to_disconnect = -1;
        }
        else
        {
            nbd_dev_to_disconnect = -1;
        }
     }

    /* Sets signal action like regular sigaction but is suspicious. */
    static int set_sigaction(int sig, const struct sigaction * act)
    {
      struct sigaction oact;
      int r = sigaction(sig, act, &oact);
      return r;
    }

    bool NBDProxy::IsValid()
    {
        return (nbd_dev_to_disconnect != -1);
    }




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

    NBDProxy::NBDProxy(u_int64_t size, const char* dev_file):
                                        BDProxyFunction(size)
    {
        //create a pair of connected sockets
        ThrowIfBad<Error>(socketpair(AF_UNIX, SOCK_STREAM, 0, m_socket_fd) == -1, "Bad Sockpair");
        //return a fd from the dev_file
        ThrowIfBad<Error>((m_nbd_fd = open(dev_file, O_RDWR)) == -1, "Bad Open");
        //set size of nbd to m_size
        ThrowIfBad<Error>(ioctl(m_nbd_fd, NBD_SET_SIZE, m_size) == -1, "Bad SetSize");
        //clear the sockets
        ThrowIfBad<Error>(ioctl(m_nbd_fd, NBD_CLEAR_SOCK) == -1, "Bad CLEAR_SOCK");

        // Registerignalsstruct
        nbd_dev_to_disconnect = m_nbd_fd;
        struct sigaction act;
        act.sa_handler = disconnect_nbd;
        act.sa_flags = SA_RESTART;


        ThrowIfBad<Error>(sigemptyset(&act.sa_mask) == -1, "Bad Sigemptyset");
        ThrowIfBad<Error>(sigaddset(&act.sa_mask, SIGINT) == -1, "Bad Sigaddset");

        set_sigaction(SIGINT, &act);

        //create detach thread
        pthread_t server_thread = 0;
        ThrowIfBad<Error>(pthread_create(&server_thread, NULL, &RunServer, this) != 0, "Bad threadcreate");
        std::cout << "thread created" << '\n';
        ThrowIfBad<Error>(pthread_detach(server_thread) != 0, "Bad Detach");

    }

    int NBDProxy::GetFd()
    {
        return m_socket_fd[0];
    }

    NBDProxy::~NBDProxy()
    {
        ThrowIfBad<Error>(close(m_socket_fd[0]) == -1, "Bad Close");
        ThrowIfBad<Error>(close(m_socket_fd[1]) == -1, "Bad Close");
        ThrowIfBad<Error>(close(m_nbd_fd) == -1, "Bad Close");
    }

    void NBDProxy::Done(struct Request &req)
    {
        //after the data was read/written
        std::cout << "done start" << '\n';
        NBDRWReq& request = (NBDRWReq&) req;

        struct nbd_reply reply;
        reply.magic = htonl(NBD_REPLY_MAGIC);
        reply.error = htonl(0);

        memcpy(reply.handle, request.m_handle, sizeof(reply.handle));
        reply.error = htonl(0);

        boost::mutex::scoped_lock lock(m_mutex);
        write_all(m_socket_fd[0], (char*)&reply, sizeof(struct nbd_reply));

        if(request.m_type == NBD_CMD_READ)
        {
           write_all(m_socket_fd[0], request.m_data, request.m_size);
           std::cout << "reading in done" << '\n';
            //takes care of the data buffer that was allcated in OnSocketReady
        }

        delete request.m_data;
        delete &req;

    }


    void NBDProxy::OnSocketReady()
    {
      std::cout << "on socket ready" << '\n';
        struct nbd_request request;
        read_all(m_socket_fd[0], (char*)&request, sizeof(nbd_request));
        // read(m_socket_fd[0], &request, sizeof(nbd_request));
        Request* my_nbd_request = 0;

        switch (ntohl(request.type))
        {
            case NBD_CMD_READ:
            {
                // std::cout << request.len << '\n';
                // std::cout << (ntohl(request.len)) << '\n';
                char *temp = new char[ntohl(request.len)];
                my_nbd_request = new NBDRWReq(request, temp);
                std::cout << "read" << '\n';
                break;
            }

            case NBD_CMD_WRITE:
            {
              std::cout << "write" << '\n';
                char *temp = new char[ntohl(request.len)];
                read_all(m_socket_fd[0], temp, ntohl(request.len));

                my_nbd_request = new NBDRWReq(request, temp);
                break;
            }

            case NBD_CMD_DISC:
            {
                my_nbd_request = new Request(request.type);
                break;
            }
        }

        std::cout << "nbd_proxy: calling m_cb(my_nbd_request, *this)" << '\n';
        (m_cb)(*my_nbd_request, *this);
        //change it so that call back will know the threadpool and activate the functor
    }

    void* NBDProxy::RunServer(void* ptr)
    {
        int sk = ((NBDProxy*)ptr)->m_socket_fd[1];
        int nbd = ((NBDProxy*)ptr)->m_nbd_fd;

        std::cout << sk << '\n';
        ThrowIfBad<Error>((ioctl(nbd, NBD_SET_SOCK, sk)) == -1, "Bad NBD_SET_SOCK");
        std::cout << "DDDDDDDDoooo it" << '\n';
        ThrowIfBad<Error>(ioctl(nbd, NBD_DO_IT) == -1, "Bad NBD_DO_IT");

        std::cout <<"DOit done"<<nbd<< '\n';

        return NULL;
    }


    NBDRWReq::NBDRWReq(struct nbd_request nbdr, char* data):
                                Request(ntohl(nbdr.type)),
                                m_size(ntohl(nbdr.len)),
                                m_offset(be64toh(nbdr.from)),
                                m_data(data)
    {
        memcpy(m_handle, nbdr.handle, 8);
    }

    NBDRWReq::~NBDRWReq(){}

    void MemoryFile::write(Request& rh)
    {
      std::cout << "Memory file: Writing" << '\n';
      NBDRWReq& write_req = static_cast<NBDRWReq&>(rh);

      lseek(descriptor, write_req.m_offset, SEEK_SET);
      write_all(descriptor, write_req.m_data, write_req.m_size);
    }

    MemoryFile::MemoryFile(int desc):Memory(desc)
    {}

    void MemoryFile::read(Request& rh)
    {
      std::cout << "MemoryFile: Reading" << '\n';
      NBDRWReq& read_req = static_cast<NBDRWReq&>(rh);
      std::cout <<"offset = "<< read_req.m_offset<<"data = " << read_req.m_data<<"size = "<<read_req.m_size<<'\n';
      lseek(descriptor, read_req.m_offset , SEEK_SET);
      read_all(descriptor, read_req.m_data, read_req.m_size);
    }


}//namespace ilrd
