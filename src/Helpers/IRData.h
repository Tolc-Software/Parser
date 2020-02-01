#pragma once

#include "IR/ir.hpp"
#include "clang/AST/DeclBase.h"
#include <deque>
#include <string>
#include <vector>

namespace Helpers {

/**
  * Used to distinguish a part name in a fully qualified name
  * Ex:
  *     NamespaceName::ClassName -> {(NamespaceName, Type::Namespace)
  *                                  (ClassName, Type::Struct)}
  */
enum class Type {
	Namespace,
	Struct,
	Unknown,
};

/**
  * Representation of a struct/class that can be used to create the IR::Struct
  */
struct Struct {
	// Fully qualified name
	// Ex:
	//     Ns::cl => {(Ns, Type::Namespace), (cl, Type::Struct)}
	std::deque<std::pair<std::string, Type>> m_name;
	// The variables within the struct/class
	std::vector<IR::Variable> m_variables;
};

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
	// {Fully qualified namespace name}
	// Ex: ParentNS::ChildNS::GrandChildNS
	std::vector<std::string> m_namespaces;
	std::vector<Struct> m_structs;
	std::vector<std::pair<clang::DeclContext const*, IR::Function>> m_functions;
};
}    // namespace Helpers
