#pragma once

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"

namespace Frontend {

class ParserFrontendAction : public clang::ASTFrontendAction {
public:
	virtual std::unique_ptr<clang::ASTConsumer>
	CreateASTConsumer(clang::CompilerInstance& Compiler,
	                  llvm::StringRef /*InFile*/);
};

}    // namespace Frontend
