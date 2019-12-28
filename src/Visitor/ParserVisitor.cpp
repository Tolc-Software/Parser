#include "Visitor/ParserVisitor.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Type.h"

#include <iostream>
#include <string>

namespace Visitor {

bool ParserVisitor::VisitFunctionDecl(clang::FunctionDecl* functionDecl) {
	for (auto& p : functionDecl->parameters()) {
		std::cout << "===== Start of variable =====" << '\n';
		std::cout << "Name: " << std::string(p->getName()) << '\n';
		std::cout << "All of the type: "
		          << clang::QualType::getAsString(p->getType().split(),
		                                          clang::PrintingPolicy {{}})
		          << '\n';
		std::cout << "Qualifiers: " << p->getType().split().Quals.getAsString()
		          << '\n';
		std::cout << "Has const: " << p->getType().split().Quals.hasConst()
		          << '\n';

		p->getType().split().Ty->dump();
		p->getType()->dump();
		p->dump();

		clang::QualifierCollector qc;
		auto withoutQs = qc.strip(p->getType());
		withoutQs->dump();
		std::cout << "===== End of variable =====" << '\n';
		std::cout << '\n';
	}
	std::cout << "I'm called with function: "
	          << functionDecl->getQualifiedNameAsString() << '\n';
	return true;
}

bool ParserVisitor::VisitNamespaceDecl(clang::NamespaceDecl* namespaceDecl) {
	std::cout << "I'm called with namespace: "
	          << namespaceDecl->getQualifiedNameAsString() << '\n';
	return true;
}

bool ParserVisitor::VisitCXXRecordDecl(clang::CXXRecordDecl* classDecl) {
	std::cout << "I'm called with: " << classDecl->getQualifiedNameAsString()
	          << '\n';
	if (classDecl->getQualifiedNameAsString() == "n::m::C") {
		clang::FullSourceLoc FullLocation =
		    m_context->getFullLoc(classDecl->getBeginLoc());
		if (FullLocation.isValid())
			llvm::outs() << "Found declaration at "
			             << FullLocation.getSpellingLineNumber() << ":"
			             << FullLocation.getSpellingColumnNumber() << "\n";
	}
	return true;
}
}    // namespace Visitor
