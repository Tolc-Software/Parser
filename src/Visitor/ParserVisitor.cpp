#include "Visitor/ParserVisitor.h"
#include "Helpers/namespaceBuilder.h"
#include <iostream>

namespace Visitor {
ParserVisitor::~ParserVisitor() {
	// Build empty namespaces and their relations
	for (auto& ns : Helpers::buildNamespaceStructure(m_irData.m_namespaces)) {
		m_parsedNamespaces.push_back(ns);
	}
}

}    // namespace Visitor
