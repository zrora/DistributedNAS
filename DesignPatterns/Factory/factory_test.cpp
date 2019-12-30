/****************************************************************************
*						Facoty Design Pattern Tests							*
*								Maor Galkin									*
*	Reviewer: 									Last update: 26.07.18		*
*****************************************************************************/
#include <cstdlib>  /*EXIT_SUCCESS*/
#include <iostream> /* Standard I/O */
#include <cstring>
#include <pthread.h>
#include <unistd.h>
#include "factory.hpp"

using namespace ilrd;
using namespace std;

inline void RunTest(bool (*test_func)(void))
{
	int result = test_func();

	if (0 == result)
	{
		cout << "[\033[0;92mTest Successful\033[0m]\n" << endl;
	}
	else
	{
		cout << "[\033[0;91mTest Failed\033[0m]\n" << endl;

		exit(result);
	}
}


enum {SUCCESS, FAILURE};

inline void RunTest(bool (*test_func)(void));
inline bool TestOne(void);
Person* CreatePerson(RawData& params);
Person* CreateStudent(RawData& params);
Person* CreateEmployee(RawData& params);


int main()
{
	RunTest(TestOne);

	return EXIT_SUCCESS;
}

inline bool TestOne(void)
{
	Factory<Person, int, RawData> factory;
	bool add_test;
	Person* p_jack = 0;

	RawData jack;
	jack.m_name = "Jack";
	jack.m_age = 34;

	RawData Amir;
	Amir.m_type = 1;
	Amir.m_name = "Amir";
	Amir.m_age = 32;
	Amir.m_selery = 10000;

	RawData Avi;
	Avi.m_type = 2;
	Avi.m_name = "Avi";
	Avi.m_age = 30;
	Avi.m_degree = "BSC";

	add_test = factory.Add(jack.m_type, CreatePerson);
	add_test = factory.Add(Amir.m_type, CreateEmployee);
	add_test = factory.Add(Avi.m_type, CreateStudent);

	p_jack = factory.Create(jack.m_type, jack);
	std::cout << p_jack->GetName()<< " is "<<p_jack->GetAge()<<" years old"<< '\n';
	operator delete (p_jack);

	p_jack = 0;
	p_jack = factory.Create(Amir.m_type, Amir);
	std::cout << p_jack->GetName()<< " is "<<p_jack->GetAge()<<" years old"<< '\n';

	operator delete (p_jack);
	p_jack = 0;
	p_jack = factory.Create(Avi.m_type, Avi);
	std::cout << p_jack->GetName()<< " is "<<p_jack->GetAge()<<" years old"<< '\n';

	operator delete (p_jack);
	p_jack = 0;


    return SUCCESS;
}


Person* CreatePerson(RawData& params)
{
	return (new Person(params.m_age, params.m_name));
}

Person* CreateStudent(RawData& params)
{
	return (new Student(params.m_age, params.m_name, params.m_degree));
}

Person* CreateEmployee(RawData& params)
{
	return (new Employee(params.m_age, params.m_name,params.m_selery));
}
