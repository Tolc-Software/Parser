#include "Visitor/ParserVisitor.h"
#include "clang/AST/Type.h"

#include <iostream>
#include <string>

namespace Visitor {

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

}
