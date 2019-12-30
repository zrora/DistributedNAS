#ifndef __SINGLETON_V1_H__
#define __SINGLETON_V1_H__

#include <boost/noncopyable.hpp>
#include "handleton.h"

namespace ilrd
{

class MyClass: boost::noncopyable
{
public:
	void Print();

private:
	friend class Handleton<MyClass>;
	MyClass();
	~MyClass();
};

}//namespace ilrd

#endif
