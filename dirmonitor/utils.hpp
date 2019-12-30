#include <iostream>

struct Error:public std::runtime_error
{
    Error(const char* _s):runtime_error(_s)
    {}
};

template<class T>
void ThrowIfBad(bool exception,const char* error)
{
    if(exception)
    {
        throw T(error);
    }
}
