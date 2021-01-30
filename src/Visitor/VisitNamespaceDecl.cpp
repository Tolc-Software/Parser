#include "Visitor/ParserVisitor.hpp"
#include "clang/AST/DeclBase.h"
#include "clang/AST/Type.h"
#include <iostream>
#include <spdlog/spdlog.h>

namespace Visitor {

bool ParserVisitor::VisitNamespaceDecl(clang::NamespaceDecl* namespaceDecl) {
	if (isInSystemHeader(namespaceDecl)) {
		// Continue the AST search
		return true;
	}
	spdlog::debug("Parsing namespace: {}",
	              namespaceDecl->getQualifiedNameAsString());

	std::cout << "On namespace: " << namespaceDecl->getQualifiedNameAsString()
	          << '\n';

	// Export our parsed namespace
	m_irData.m_namespaces.push_back(namespaceDecl->getQualifiedNameAsString());

	// Continue the AST search
	return true;
}

}
