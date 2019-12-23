#include "Visitor/ParserVisitor.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

#include <iostream>
#include <string>

using namespace clang;

class ParserConsumer : public clang::ASTConsumer {
public:
	explicit ParserConsumer(ASTContext* context) : m_visitor(context) {}

	virtual void HandleTranslationUnit(clang::ASTContext& context) {
		m_visitor.TraverseDecl(context.getTranslationUnitDecl());
	}

private:
	Visitor::ParserVisitor m_visitor;
};

class ParserAction : public clang::ASTFrontendAction {
public:
	virtual std::unique_ptr<clang::ASTConsumer>
	CreateASTConsumer(clang::CompilerInstance& Compiler,
	                  llvm::StringRef /*InFile*/) {
		return std::unique_ptr<clang::ASTConsumer>(
		    new ParserConsumer(&Compiler.getASTContext()));
	}
};

int main(int /*argc*/, char** /*argv*/) {
	// if (argc > 1) {
	// 	clang::tooling::runToolOnCode(new ParserAction(), argv[1]);
	// }

	// clang::tooling::runToolOnCode(
	//     new ParserAction(),
	//     "namespace n { namespace m { class C {}; } }");

	clang::tooling::runToolOnCode(new ParserAction(),
	                              R"(
int FunctionName(const double d, int i) {
	return d + 5;
}
		)");
}
