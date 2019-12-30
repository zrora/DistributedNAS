#ifndef __MEMORY
#define __MEMORY

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
// #include "nbdproxy.hpp"
#include "request.hpp"

namespace ilrd
{
class Memory
{
public:
  Memory(int desc);
  virtual void read (Request&) = 0;
  virtual void write (Request&) = 0;
  int descriptor;
};

class MemoryFile:public Memory
{
public:
  MemoryFile(int desc);
  void read(Request&);
  void write(Request&);
};


}//namespace ilrd

#endif
