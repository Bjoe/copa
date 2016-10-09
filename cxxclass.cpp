#include "cxxclass.hpp"

#include "llvm/Support/raw_os_ostream.h"

namespace mockery {

CxxClass::CxxClass(std::string className) : className_(className)
{
}

void CxxClass::setClassName(std::string name)
{
	className_ = name;
}

void CxxClass::addNamespace(std::string name)
{
	nameSpaces_.insert(name);
}

void CxxClass::addMethod(CxxMethod method)
{
	methods_.push_back(method);
}

std::string CxxClass::printOut()
{
	std::string source;
	llvm::raw_string_ostream sourceStream(source);

	for(std::string name : nameSpaces_)
	{
		sourceStream << "namespace " << name << " {\n";
	}

	//sourceStream << "class " << className_ << "\n{\n";

	for(CxxMethod method : methods_)
	{
		sourceStream << method.method();
	}

	//sourceStream << "\n};\n";

	for(std::string name : nameSpaces_)
	{
		sourceStream << "} // namespace " << name;
	}

	return sourceStream.str();
}

} // namespace mockery
