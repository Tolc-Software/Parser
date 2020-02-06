#include "Visitor/ParserVisitor.h"
#include "Builders/functionBuilder.h"
#include "Builders/namespaceBuilder.h"
#include "Builders/structBuilder.h"

namespace Visitor {
ParserVisitor::~ParserVisitor() {
	// Build empty namespaces and their relations
	m_parsedNamespaces =
	    Builders::buildNamespaceStructure(m_irData.m_namespaces);

	// Add the structs/classes to the namespaces
	Builders::buildStructs(m_irData.m_structs, m_parsedNamespaces);

	// Add the functions to the namespaces
	Builders::buildFunctions(m_irData.m_functions, m_parsedNamespaces);
}

}    // namespace Visitor
