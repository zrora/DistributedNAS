#ifndef __SHAREDOBJ
#define __SHAREDOBJ
#include <string>
#include <boost/core/noncopyable.hpp>
namespace ilrd
{
class SharedObject:private boost::noncopyable
{
public:
  explicit SharedObject(const std::string path);
  ~SharedObject();

  template<typename S>
  S GetSymbol(const std::string symbol);

private:
  void* m_handle;
  // const std::string& m_soname;
};

}//namespace ilrd
#endif
