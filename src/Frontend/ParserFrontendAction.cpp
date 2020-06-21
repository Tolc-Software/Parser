#include "Frontend/ParserFrontendAction.hpp"
#include "Consumer/ParserConsumer.hpp"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"

namespace Frontend {

std::unique_ptr<clang::ASTConsumer>
ParserFrontendAction::CreateASTConsumer(clang::CompilerInstance& compiler,
                                        llvm::StringRef /*InFile*/) {
	return std::unique_ptr<clang::ASTConsumer>(new Consumer::ParserConsumer(
	    &compiler.getASTContext(), m_parsedNamespaces, m_parsedSuccessfully));
}

}    // namespace Frontend
