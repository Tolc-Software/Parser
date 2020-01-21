#include "Helpers/namespaceBuilder.h"
#include "IR/ir.hpp"
#include "Visitor/ParserVisitor.h"
#include "clang/AST/DeclBase.h"
#include "clang/AST/Type.h"

#include <iostream>
#include <string>

namespace Visitor {

bool ParserVisitor::VisitNamespaceDecl(clang::NamespaceDecl* namespaceDecl) {
	IR::Namespace ns;
	ns.m_name = namespaceDecl->getName();

	// Export our parsed namespace
	m_irData.m_namespaces.push_back(
	    std::make_pair(Helpers::getParentNamespaceName(namespaceDecl), ns));

	// Continue the AST search
	return true;
}

}
