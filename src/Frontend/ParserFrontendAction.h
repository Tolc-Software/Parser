#pragma once

#include "IR/ir.hpp"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include <vector>

namespace Frontend {

class ParserFrontendAction : public clang::ASTFrontendAction {
public:
	explicit ParserFrontendAction(std::vector<IR::Namespace>& parsedNamespaces)
	    : m_parsedNamespaces(parsedNamespaces) {}

	std::unique_ptr<clang::ASTConsumer>
	CreateASTConsumer(clang::CompilerInstance& Compiler,
	                  llvm::StringRef /*InFile*/);

	std::vector<IR::Namespace>& m_parsedNamespaces;
};

}    // namespace Frontend
