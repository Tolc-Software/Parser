#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

#include <iostream>
#include <string>

using namespace clang;

class FindNamedClassVisitor
    : public RecursiveASTVisitor<FindNamedClassVisitor> {
public:
	explicit FindNamedClassVisitor(ASTContext* Context) : Context(Context) {}

	bool VisitFunctionDecl(FunctionDecl* Declaration) {
		for (auto& p : Declaration->parameters()) {
			std::cout << "===== Start of variable =====" << '\n';
			std::cout << "Name: " << std::string(p->getName()) << '\n';
			std::cout << "All of the type: "
			          << QualType::getAsString(p->getType().split(),
			                                   PrintingPolicy {{}})
			          << '\n';
			LangOptions lops;
			lops.CPlusPlus = true;
			auto pp = PrintingPolicy(lops);
			std::cout << "With custom policy: "
			          << QualType::getAsString(p->getType().split(), pp)
			          << '\n';
			std::cout << "Qualifiers: "
			          << p->getType().split().Quals.getAsString() << '\n';
			std::cout << "Has const: " << p->getType().split().Quals.hasConst()
			          << '\n';

			p->getType().split().Ty->dump();
			p->getType()->dump();
			p->dump();
			std::cout << "===== End of variable =====" << '\n';
			std::cout << '\n';
		}
		std::cout << "I'm called with function: "
		          << Declaration->getQualifiedNameAsString() << '\n';
		return true;
	}

	bool VisitNamespaceDecl(NamespaceDecl* Declaration) {
		std::cout << "I'm called with namespace: "
		          << Declaration->getQualifiedNameAsString() << '\n';
		return true;
	}

	bool VisitCXXRecordDecl(CXXRecordDecl* Declaration) {
		std::cout << "I'm called with: "
		          << Declaration->getQualifiedNameAsString() << '\n';
		if (Declaration->getQualifiedNameAsString() == "n::m::C") {
			FullSourceLoc FullLocation =
			    Context->getFullLoc(Declaration->getBeginLoc());
			if (FullLocation.isValid())
				llvm::outs() << "Found declaration at "
				             << FullLocation.getSpellingLineNumber() << ":"
				             << FullLocation.getSpellingColumnNumber() << "\n";
		}
		return true;
	}

private:
	ASTContext* Context;
};

class FindNamedClassConsumer : public clang::ASTConsumer {
public:
	explicit FindNamedClassConsumer(ASTContext* Context) : Visitor(Context) {}

	virtual void HandleTranslationUnit(clang::ASTContext& Context) {
		Visitor.TraverseDecl(Context.getTranslationUnitDecl());
	}

private:
	FindNamedClassVisitor Visitor;
};

class FindNamedClassAction : public clang::ASTFrontendAction {
public:
	virtual std::unique_ptr<clang::ASTConsumer>
	CreateASTConsumer(clang::CompilerInstance& Compiler,
	                  llvm::StringRef /*InFile*/) {
		return std::unique_ptr<clang::ASTConsumer>(
		    new FindNamedClassConsumer(&Compiler.getASTContext()));
	}
};

int main(int /*argc*/, char** /*argv*/) {
	// if (argc > 1) {
	// 	clang::tooling::runToolOnCode(new FindNamedClassAction(), argv[1]);
	// }

	// clang::tooling::runToolOnCode(
	//     new FindNamedClassAction(),
	//     "namespace n { namespace m { class C {}; } }");

	clang::tooling::runToolOnCode(new FindNamedClassAction(),
	                              R"(
int FunctionName(const double d, int i) {
	return d + 5;
}
		)");
}
