#include "Consumer/ParserConsumer.h"
#include "Visitor/ParserVisitor.h"
#include "clang/AST/ASTConsumer.h"

namespace Consumer {

void ParserConsumer::HandleTranslationUnit(clang::ASTContext& context) {
	m_visitor.TraverseDecl(context.getTranslationUnitDecl());
}

}    // namespace Consumer
