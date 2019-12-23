#pragma once
#include "clang/AST/RecursiveASTVisitor.h"

namespace Visitor {
class ParserVisitor : public clang::RecursiveASTVisitor<ParserVisitor> {
public:
	explicit ParserVisitor(clang::ASTContext* context) : m_context(context) {}

	bool VisitFunctionDecl(clang::FunctionDecl* functionDecl);

	bool VisitNamespaceDecl(clang::NamespaceDecl* namespaceDecl);

	bool VisitCXXRecordDecl(clang::CXXRecordDecl* classDecl);

private:
	clang::ASTContext* m_context;
};
}    // namespace Visitor
