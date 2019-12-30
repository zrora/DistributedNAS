#ifndef __FACTORY_HPP__
#define __FACTORY_HPP__

#include <map>
#include <iostream>
namespace ilrd
{

template<class T, class Key, class Params>
class Factory
{
private:
    typedef T *(*creation_func_t)(Params& params);
    std::map<Key, creation_func_t> m_map;

public:
    bool Add(const Key& key, creation_func_t CreateFunc);
    T* Create(const Key& key, Params& params);
};

template<class T, class Key, class Params>
bool Factory<T, Key, Params>::Add(const Key& key, creation_func_t CreateFunc)
{
  //The pair::second element in the pair is set to true if a new element was
  // inserted or false if an equivalent key already existed.
  std::cout << "addded to :" << this << '\n';
  return m_map.insert(std::pair<Key, creation_func_t>(key, CreateFunc)).second;//insert return value is a pair <iterator , bool>
}

template<class T , class Key ,class Params>
T* Factory<T, Key, Params>::Create(const Key& key, Params& params)
{
  //Find the correct ctor function from the map with the key!
  typename std::map<Key, creation_func_t>::iterator it = m_map.find(key);
  //activate the creation function (second of the pair stored in map)
  // T* ret =
  //Create a new T class(parent) instance
  // ret =
  return it->second(params);;
}

class Person
{
public:
    Person(int age, std::string name) : m_age(age), m_name(name){}
    virtual ~Person(){};
    int GetAge()
    {
        return m_age;
    }
    std::string GetName()
    {
        return m_name;
    }
private:
    int m_age;
    std::string m_name;
};

class Employee : public Person
{
public:
    ~Employee(){};
    Employee(int age, std::string name, int selery) : Person(age, name)
    , m_selery(selery){}
    int GetSelery()
    {
        return m_selery;
    }
private:
    int m_selery;
};

class Student : public Person
{
public:
    ~Student(){};
    Student(int age, std::string name, std::string degree) : Person(age, name)
    , m_degree(degree){}
    std::string GetDegreeLvl()
    {
        return m_degree;
    }
private:
    std::string m_degree;
};

struct RawData
{
    RawData() : m_type(0), m_name(""), m_age(0), m_degree(""), m_selery(0){}
	int m_type; //0 == Person, 1 == Employee, 2 == Student
	std::string m_name;
    int m_age;
	std::string m_degree;
	int m_selery;
};

} // namespace ilrd

#endif //__FACTORY_HPP__
