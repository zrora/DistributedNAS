#ifndef __REQUEST
#define __REQUEST

namespace ilrd
{
typedef struct Request
{
    Request(int type);
    virtual ~Request();
    int m_type;
}Request;

}//namespace ilrd

#endif
