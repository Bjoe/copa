#ifndef MOCKERY_CXXMETHOD_HPP
#define MOCKERY_CXXMETHOD_HPP

#include <string>
#include <list>

namespace mockery {

class CxxMethod
{
public:
  CxxMethod(std::string className, std::string methodName);

  void setReturnType(std::string name);
  void addParameter(std::string typeName, std::string name);
  std::string method();

private:
  std::string className_;
  std::string methodName_;
  std::string returnType_;
  std::list<std::string> parameter_;
};

} // namespace mockery

#endif // MOCKERY_CXXMETHOD_HPP
