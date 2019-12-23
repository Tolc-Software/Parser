#include "Visitor/ParserVisitor.h"
#include "clang/AST/RecursiveASTVisitor.h"

#include <iostream>
#include <string>

namespace Visitor {

bool ParserVisitor::VisitFunctionDecl(clang::FunctionDecl* Declaration) {
	for (auto& p : Declaration->parameters()) {
		std::cout << "===== Start of variable =====" << '\n';
		std::cout << "Name: " << std::string(p->getName()) << '\n';
		std::cout << "All of the type: "
		          << clang::QualType::getAsString(p->getType().split(),
		                                          clang::PrintingPolicy {{}})
		          << '\n';
		clang::LangOptions lops;
		lops.CPlusPlus = true;
		auto pp = clang::PrintingPolicy(lops);
		std::cout << "With custom policy: "
		          << clang::QualType::getAsString(p->getType().split(), pp)
		          << '\n';
		std::cout << "Qualifiers: " << p->getType().split().Quals.getAsString()
		          << '\n';
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

bool ParserVisitor::VisitNamespaceDecl(clang::NamespaceDecl* Declaration) {
	std::cout << "I'm called with namespace: "
	          << Declaration->getQualifiedNameAsString() << '\n';
	return true;
}

bool ParserVisitor::VisitCXXRecordDecl(clang::CXXRecordDecl* Declaration) {
	std::cout << "I'm called with: " << Declaration->getQualifiedNameAsString()
	          << '\n';
	if (Declaration->getQualifiedNameAsString() == "n::m::C") {
		clang::FullSourceLoc FullLocation =
		    m_context->getFullLoc(Declaration->getBeginLoc());
		if (FullLocation.isValid())
			llvm::outs() << "Found declaration at "
			             << FullLocation.getSpellingLineNumber() << ":"
			             << FullLocation.getSpellingColumnNumber() << "\n";
	}
	return true;
}
}    // namespace Visitor
