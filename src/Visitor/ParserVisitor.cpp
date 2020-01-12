#include "Visitor/ParserVisitor.h"

namespace Visitor {
ParserVisitor::~ParserVisitor() {
	// TODO: Transform m_irData to m_parsedNamespaces
	for (auto& [parent, ns] : m_irData.m_namespaces) {
		m_parsedNamespaces.push_back(ns);
	}
}

}    // namespace Visitor
