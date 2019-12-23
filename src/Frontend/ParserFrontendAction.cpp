#include "Frontend/ParserFrontendAction.h"
#include "Consumer/ParserConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"

namespace Frontend {

std::unique_ptr<clang::ASTConsumer>
ParserFrontendAction::CreateASTConsumer(clang::CompilerInstance& Compiler,
                                        llvm::StringRef /*InFile*/) {
	return std::unique_ptr<clang::ASTConsumer>(
	    new Consumer::ParserConsumer(&Compiler.getASTContext()));
}

}    // namespace Frontend
