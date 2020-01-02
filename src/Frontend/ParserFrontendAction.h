#pragma once

#include "Helpers/IRData.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"

namespace Frontend {

class ParserFrontendAction : public clang::ASTFrontendAction {
public:
	explicit ParserFrontendAction(Helpers::IRData& irData) : m_irData(irData) {}

	std::unique_ptr<clang::ASTConsumer>
	CreateASTConsumer(clang::CompilerInstance& Compiler,
	                  llvm::StringRef /*InFile*/);

	Helpers::IRData& m_irData;
};

}    // namespace Frontend
