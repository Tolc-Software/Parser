#include "Visitor/ParserVisitor.h"
#include "clang/AST/DeclBase.h"
#include "clang/AST/Type.h"

namespace Visitor {

bool ParserVisitor::VisitNamespaceDecl(clang::NamespaceDecl* namespaceDecl) {
	// Export our parsed namespace
	m_irData.m_namespaces.push_back(namespaceDecl->getQualifiedNameAsString());

	// Continue the AST search
	return true;
}

}
