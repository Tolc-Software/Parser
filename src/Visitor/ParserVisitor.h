#pragma once

#include "Helpers/IRData.h"
#include "clang/AST/RecursiveASTVisitor.h"

namespace Visitor {
class ParserVisitor : public clang::RecursiveASTVisitor<ParserVisitor> {
public:
	explicit ParserVisitor(clang::ASTContext* context, Helpers::IRData& irData)
	    : m_context(context), m_irData(irData) {}

	bool VisitFunctionDecl(clang::FunctionDecl* functionDecl);

	bool VisitNamespaceDecl(clang::NamespaceDecl* namespaceDecl);

	bool VisitCXXRecordDecl(clang::CXXRecordDecl* classDecl);

private:
	clang::ASTContext* m_context;
	Helpers::IRData& m_irData;
};
}    // namespace Visitor
