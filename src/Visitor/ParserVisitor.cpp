#include "Visitor/ParserVisitor.hpp"
#include "Builders/enumBuilder.hpp"
#include "Builders/functionBuilder.hpp"
#include "Builders/namespaceBuilder.hpp"
#include "Builders/structBuilder.hpp"

namespace Visitor {
ParserVisitor::~ParserVisitor() {
	if (m_parsedSuccessfully) {
		// Build empty namespaces and their relations
		m_parsedNamespaces =
		    Builders::buildNamespaceStructure(m_irData.m_namespaces);

		// Add the collected member variables to the structs
		Builders::addMemberVariables(m_irData.m_structs,
		                             m_irData.m_memberVariables);

		// Add the structs/classes to the namespaces
		Builders::buildStructs(m_irData.m_structs, m_parsedNamespaces);

		// Add the functions to the namespaces
		Builders::buildFunctions(m_irData.m_functions, m_parsedNamespaces);

		// Add the enums to the namespaces
		Builders::buildEnums(m_irData.m_enums, m_parsedNamespaces);
	}
}

}    // namespace Visitor
