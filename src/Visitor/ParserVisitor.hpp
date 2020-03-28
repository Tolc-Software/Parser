#pragma once

#include "IR/ir.hpp"
#include "IRProxy/IRData.hpp"
#include "clang/AST/RecursiveASTVisitor.h"

namespace Visitor {
class ParserVisitor : public clang::RecursiveASTVisitor<ParserVisitor> {
public:
	explicit ParserVisitor(clang::ASTContext* context,
	                       IR::Namespace& parsedNamespaces)
	    : m_context(context), m_parsedNamespaces(parsedNamespaces) {}

	// Will populate the m_parsedNamespaces
	// since it is guaranteed that all visitors will have run by then
	~ParserVisitor();

	bool VisitFunctionDecl(clang::FunctionDecl* functionDecl);

	bool VisitNamespaceDecl(clang::NamespaceDecl* namespaceDecl);

	bool VisitCXXRecordDecl(clang::CXXRecordDecl* classDecl);

	bool VisitFieldDecl(clang::FieldDecl* fieldDecl);

private:
	clang::ASTContext* m_context;

	// This is what the visitor functions need to fill
	IRProxy::IRData m_irData;

	// This will get filled in the destructor of the visitor
	IR::Namespace& m_parsedNamespaces;

	bool isInSystemHeader(clang::Decl* decl) {
		return m_context->getSourceManager().isInSystemHeader(
		    decl->getLocation());
	}
};
}    // namespace Visitor
