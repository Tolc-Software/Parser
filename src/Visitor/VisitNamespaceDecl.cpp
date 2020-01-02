#include "Visitor/ParserVisitor.h"
#include "clang/AST/Type.h"

#include <iostream>
#include <string>

namespace Visitor {

bool ParserVisitor::VisitNamespaceDecl(clang::NamespaceDecl* namespaceDecl) {
	std::cout << "I'm called with namespace: "
	          << namespaceDecl->getQualifiedNameAsString() << '\n';
	return true;
}

}
