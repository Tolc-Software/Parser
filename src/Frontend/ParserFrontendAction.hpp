#pragma once

#include "IR/ir.hpp"
#include "Parser/MetaData.hpp"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include <vector>

namespace Frontend {

class ParserFrontendAction : public clang::ASTFrontendAction {
public:
	explicit ParserFrontendAction(IR::Namespace& parsedNamespaces,
	                              Parser::MetaData& metaData,
	                              bool& parsedSuccessfully);

	std::unique_ptr<clang::ASTConsumer>
	CreateASTConsumer(clang::CompilerInstance& compiler,
	                  llvm::StringRef /*InFile*/);

	IR::Namespace& m_parsedNamespaces;
	Parser::MetaData& m_metaData;
	bool& m_parsedSuccessfully;
};

}    // namespace Frontend
