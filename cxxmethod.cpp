#include "cxxmethod.hpp"

#include "llvm/Support/raw_os_ostream.h"

namespace mockery {

CxxMethod::CxxMethod(std::string className, std::string methodName) : className_(className), methodName_(methodName)
{}

void CxxMethod::setReturnType(std::string name)
{
	returnType_ = name;
}

void CxxMethod::addParameter(std::string typeName, std::string name)
{
	std::string parameter = typeName + " " + name;
	parameter_.push_back(parameter);
}

std::string CxxMethod::method()
{
	std::string source;
	llvm::raw_string_ostream sourceStream(source);
	sourceStream << returnType_ << " " << className_ << "::" << methodName_ << "(";
	if(0 < parameter_.size())
	{
		sourceStream << parameter_.front();
		parameter_.pop_front();
	}
	for(std::string param : parameter_)
	{
		sourceStream << ", " << param;
	}
	sourceStream << ")\n{\n\n\n\n\n}\n";
	return sourceStream.str();
}

} // namespace mockery
