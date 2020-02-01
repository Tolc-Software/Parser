#include "Visitor/ParserVisitor.h"
#include "Helpers/Builders/namespaceBuilder.h"
#include "Helpers/Builders/structBuilder.h"
#include "Helpers/IRData.h"
#include "IR/ir.hpp"

namespace Visitor {
ParserVisitor::~ParserVisitor() {
	// Build empty namespaces and their relations
	m_parsedNamespaces =
	    Helpers::Builders::buildNamespaceStructure(m_irData.m_namespaces);

	// Add the structs/classes to the namespaces
	Helpers::Builders::buildStructs(m_irData.m_structs, m_parsedNamespaces);
}

}    // namespace Visitor
