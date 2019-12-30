
#include "myclass.h"

void func(ilrd::MyClass &m)
{
	m.Print();
}

int main(void)
{
	using namespace ilrd;
	Handleton<MyClass> h;
	MyClass& m = h;
	m.Print();
	h->Print();
	func(h);
	return 0;
}
