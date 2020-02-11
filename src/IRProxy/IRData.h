#pragma once

#include "IR/ir.hpp"
#include <deque>
#include <string>
#include <vector>

namespace IRProxy {

/**
  * Used to distinguish a part name in a fully qualified name
  * Ex:
  *     NamespaceName::ClassName -> {(NamespaceName, Structure::Namespace)
  *                                  (ClassName, Structure::Struct)}
  */
enum class Structure {
	Namespace,
	Struct,
	Function,
};

/**
  * Representation of a struct/class that can be used to create the IR::Struct
  */
struct Struct {
	// Fully qualified name
	// Ex:
	//     Ns::cl => {(Ns, Structure::Namespace), (cl, Structure::Struct)}
	std::deque<std::pair<std::string, Structure>> m_name;
	// The variables within the struct/class
	std::vector<IR::Variable> m_variables;
};

/**
  * Representation of a function that can be used to create the IR::Function
  */
struct Function {
	// Fully qualified name
	// Ex:
	//     Ns::cl::fun => {(Ns, Structure::Namespace), (cl, Structure::Struct), (cl, Structure::Function)}
	std::deque<std::pair<std::string, Structure>> m_name;
	// The arguments to the function
	std::vector<IR::Variable> m_arguments;

	IR::Type m_returnType;
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
	std::vector<std::pair<std::optional<IR::AccessModifier>, Function>>
	    m_functions;
};
}    // namespace IRProxy

