#ifndef MOCKERY_CXXCLASS_HPP
#define MOCKERY_CXXCLASS_HPP

#include <string>
#include <list>
#include <set>

#include "cxxmethod.hpp"

namespace mockery {

class CxxClass
{
public:
  CxxClass(std::string className);

  void setClassName(std::string name);
  void addNamespace(std::string name);
  void addMethod(CxxMethod method);

  std::string printOut();

private:
  std::string className_;
  std::set<std::string> nameSpaces_;
  std::list<CxxMethod> methods_;
};

} // namespace mockery

#endif // MOCKERY_CXXCLASS_HPP
