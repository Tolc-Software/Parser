#pragma once

#include "IR/ir.hpp"
#include "clang/AST/DeclBase.h"
#include <string>
#include <vector>

namespace Helpers {

/**
  * Contains partially parsed code to IR on the form
  *     m_something = vector<[parent context, IR::something]>
  *
  * NOTE: Each 'm_something' should ONLY be touched by their corresponding visit function.
  *       Ex:
  *           m_namespaces should only be added to in the VisitNamespaceDecl function
  *
  * NOTE: The parent context might be a null pointer or otherwise empty
  *       if 'something' is in the global scope
  */
struct IRData {
	// {parent namespace name, namespace}
	std::vector<std::pair<std::string, IR::Namespace>> m_namespaces;
	std::vector<std::pair<clang::DeclContext const*, IR::Function>> m_functions;
};
}
