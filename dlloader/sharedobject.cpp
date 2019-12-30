#include "sharedobject.hpp"
#include <dlfcn.h>
#include <iostream>//cout
using namespace ilrd;

SharedObject::SharedObject(const std::string path)
{
  if(std::string::npos != path.find(".so"))
  {
    m_handle = dlopen(path.data(),RTLD_LAZY);
    std::cout << "SharedObject: opening file: " <<path.data()<< "handle = "<<m_handle<< '\n';
  }
}

SharedObject::~SharedObject()
{
  dlclose(m_handle);
}

template<typename S>
S SharedObject::GetSymbol(const std::string path)
{
  return dlsym(m_handle, path.c_str());
}
