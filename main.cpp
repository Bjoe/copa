
#include <string>
#include <memory>
#include <iostream>
#include <map>

#include "clang/Driver/Options.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

#include "clang/Rewrite/Core/Rewriter.h"

#include "cxxclass.hpp"
#include "cxxmethod.hpp"

using namespace std;
using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace clang::ast_matchers;
using namespace llvm;

static 	cl::OptionCategory category("Tool options");


class ExampleVisitor : public RecursiveASTVisitor<ExampleVisitor> {
private:
	ASTContext *astContext; // used for getting additional AST info

public:
	explicit ExampleVisitor(CompilerInstance *compilerInstance)
		: astContext(&(compilerInstance->getASTContext())) // initialize private members
	{
	}

	virtual bool VisitDecl(Decl *decl) {
		std::cout << "============ VisitDecl ================== " << std::endl;
		decl->dumpColor();
		return true;
	}

	virtual bool VisitCXXRecordDecl(CXXRecordDecl *decl) {
		std::cout << "============ VisitCXXRecordDecl ================== " << std::endl;
		decl->dumpColor();
		return true;
	}

	virtual bool VisitFunctionDecl(FunctionDecl *func) {
		std::cout << "============ VisitFunctionDecl ================== " << std::endl;
		func->dumpColor();
		return true;
	}

	virtual bool VisitStmt(Stmt *st) {
		std::cout << "============ VisitStmt ================== " << std::endl;
		st->dumpColor();
		return true;
	}

	/*
	virtual bool VisitReturnStmt(ReturnStmt *ret) {
		rewriter.ReplaceText(ret->getRetValue()->getLocStart(), 6, "val");
		errs() << "** Rewrote ReturnStmt\n";
		return true;
	}
	virtual bool VisitCallExpr(CallExpr *call) {
		rewriter.ReplaceText(call->getLocStart(), 7, "add5");
		errs() << "** Rewrote function call\n";
		return true;
	}
*/
};



class ExampleASTConsumer : public ASTConsumer {
private:
	ExampleVisitor *visitor; // doesn't have to be private

public:
	// override the constructor in order to pass CI
	explicit ExampleASTConsumer(CompilerInstance *CI)
		: visitor(new ExampleVisitor(CI)) // initialize the visitor
	{ }

	// override this to call our ExampleVisitor on the entire source file
	virtual void HandleTranslationUnit(ASTContext &Context) {
		/* we can use ASTContext to get the TranslationUnitDecl, which is
			 a single Decl that collectively represents the entire source file */
		visitor->TraverseDecl(Context.getTranslationUnitDecl());

		//llvm::raw_fd_ostream myfile(filename, error_code, llvm::sys::fs::F_Append);
	}

	/*
	// override this to call our ExampleVisitor on each top-level Decl
	virtual bool HandleTopLevelDecl(DeclGroupRef DG) {
		// a DeclGroupRef may have multiple Decls, so we iterate through each one
		for (DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; i++) {
			Decl *D = *i;
			visitor->TraverseDecl(D); // recursively visit each AST node in Decl "D"
		}
		return true;
	}
*/
};



class ExampleFrontendAction : public ASTFrontendAction {
public:
	virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &compilerInstance, StringRef file) {
		return std::unique_ptr<ExampleASTConsumer>(new ExampleASTConsumer(&compilerInstance)); // pass CI pointer to ASTConsumer
	}
};

std::map<std::string, mockery::CxxClass> clazzes_;

DeclarationMatcher matcher = cxxMethodDecl(isPublic(), unless(isImplicit()), unless(isDefinition())).bind("methods");

class Printer : public MatchFinder::MatchCallback
{
public:
	virtual void run(const MatchFinder::MatchResult &result)
	{
		if (const CXXRecordDecl* clazz = result.Nodes.getNodeAs<clang::CXXRecordDecl>("clazz"))
		{
			std::cout << "Class........... " << clazz->getNameAsString() << std::endl;
			//clazz->dumpColor();

			for(auto method : clazz->methods())
			{
				std::cout << "Method.......... " << method->getNameAsString() << std::endl;
				//method->dumpColor();
			}
		}

		if (const CXXMethodDecl* method = result.Nodes.getNodeAs<clang::CXXMethodDecl>("methods"))
		{
			const CXXRecordDecl* clazz = method->getParent();
			std::string className = clazz->getName().str();
			if(std::string("MyClass").compare(className) == 0)
			{
				mockery::CxxClass cxxClass(className);
				mockery::CxxMethod cxxMethod(className, method->getName().str());
				auto search = clazzes_.find(className);
				if(search != clazzes_.end())
				{
					cxxClass = search->second;
					clazzes_.erase(search);
				}

				const DeclContext* parent = clazz->getEnclosingNamespaceContext();
				while(parent && parent->isNamespace())
				{
					const NamespaceDecl* namesp = dyn_cast<NamespaceDecl>(parent);
					//namesp->dumpColor();
					//std::cout << "Namespace....... " << namesp->getName().str() << std::endl;
					//sourceStream << "namespace ";
					//namesp->printName(sourceStream);
					//sourceStream << " {\n";
					cxxClass.addNamespace(namesp->getName().str());
					//parent = parent->getParent();

					parent = parent->getParent();
				}

				//std::cout << "Class........... " << clazz->getNameAsString() << std::endl;
				//clazz->dumpColor();


				QualType retType = method->getReturnType();
				//sourceStream << retType.getAsString() << " ";
				//clazz->printName(sourceStream);
				//sourceStream << "::";

				cxxMethod.setReturnType(retType.getAsString());

				//std::cout << "Method.......... " << method->getNameAsString() << std::endl;
				//method->dumpColor();
				//method->printName(sourceStream);
				//sourceStream << "(";

				for(auto param : method->parameters())
				{
					//std::cout << "Parameter....... " << param->getNameAsString() << std::endl;
					//param->dumpColor();
					QualType type = param->getType();

					cxxMethod.addParameter(type.getAsString(), param->getName().str());

					//std::cout << "Type............ " << type.getAsString() << std::endl;
					//sourceStream << type.getAsString() << " ";
					//param->printName(sourceStream);
					//sourceStream << ", ";

					//SplitQualType splitQualType = type.split();
					//std::cout << "Quals " << splitQualType.Quals.getAsString() << std::endl;
					//splitQualType.Ty->dump();
					//type.dump();
					//std::cout << "Type " << type.dump( << std::endl;
				}
				//sourceStream << ")\n{\n\n\n}\n";

				//std::cout << sourceStream.str();

				cxxClass.addMethod(cxxMethod);
				clazzes_.insert({className, cxxClass});
			}
		}
	}
};


int main(int argc, const char **argv) {
	CommonOptionsParser op(argc, argv, category);
	ClangTool tool(op.getCompilations(), op.getSourcePathList());

	Printer printer;
	MatchFinder finder;
	finder.addMatcher(matcher, &printer);

	//return tool.run(newFrontendActionFactory<ExampleFrontendAction>().get());
	int result = tool.run(newFrontendActionFactory(&finder).get());

	for(auto item : clazzes_)
	{
		mockery::CxxClass clazz = item.second;
		std::cout << clazz.printOut() << "\n";
	}

	return result;
}
