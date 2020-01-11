#include "IR/ir.hpp"
#include "Visitor/ParserVisitor.h"
#include "clang/AST/DeclBase.h"
#include "clang/AST/Type.h"

#include <iostream>
#include <string>

namespace Visitor {

bool ParserVisitor::VisitNamespaceDecl(clang::NamespaceDecl* namespaceDecl) {
	std::cout << "I'm called with qualified namespace: "
	          << namespaceDecl->getQualifiedNameAsString() << '\n';
	std::cout << "I'm called with namespace: "
	          << namespaceDecl->getNameAsString() << '\n';

	IR::Namespace ns;
	ns.m_name = namespaceDecl->getNameAsString();

	// Export our parsed namespace
	m_irData.m_namespaces.push_back(
	    std::make_pair(namespaceDecl->getParent(), ns));

	// Continue the AST search
	return true;
}

}
