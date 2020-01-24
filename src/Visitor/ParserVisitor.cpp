#include "Visitor/ParserVisitor.h"
#include "Helpers/namespaceBuilder.h"
#include <iostream>

namespace Visitor {
ParserVisitor::~ParserVisitor() {
	// Build empty namespaces and their relations
	m_parsedNamespaces =
	    Helpers::buildNamespaceStructure(m_irData.m_namespaces);
}

}    // namespace Visitor
