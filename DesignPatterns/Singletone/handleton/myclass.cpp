#include "handleton.h"
#include "myclass.h"

namespace ilrd
{


MyClass::MyClass()
{
	std::cout<<"In CTOR MyClass"<<std::endl;
}

MyClass::~MyClass()
{
	std::cout<<"In DTOR MyClass"<<std::endl;
}

void MyClass::Print()
{
	std::cout<<"Printing!"<<std::endl;
}

IMPLEMENT_SINGLETON(MyClass);


}//namespace ilrd
