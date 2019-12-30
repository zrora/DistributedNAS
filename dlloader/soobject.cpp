#include <iostream>

__attribute__((constructor))
void NewSoFile()
{
  std::cout << "new so file was loaded " << '\n';
}

__attribute__((destructor))
void SoFileDestructor()
{
  std::cout << "/* attribute destructor */" << '\n';
}
