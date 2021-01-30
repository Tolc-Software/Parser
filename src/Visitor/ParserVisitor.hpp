#pragma once

#include "IRProxy/IRData.hpp"
#include <IR/ir.hpp>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>

namespace Visitor {
class ParserVisitor : public clang::RecursiveASTVisitor<ParserVisitor> {
public:
	explicit ParserVisitor(clang::ASTContext* context,
	                       IR::Namespace& parsedNamespaces,
	                       bool& parsedSuccessfully)
	    : m_context(context), m_parsedNamespaces(parsedNamespaces),
	      m_parsedSuccessfully(parsedSuccessfully) {}

	// Will populate the m_parsedNamespaces
	// since it is guaranteed that all visitors will have run by then
	~ParserVisitor();

	bool VisitFunctionDecl(clang::FunctionDecl* functionDecl);

	bool VisitNamespaceDecl(clang::NamespaceDecl* namespaceDecl);

	bool VisitCXXRecordDecl(clang::CXXRecordDecl* classDecl);

	bool VisitFieldDecl(clang::FieldDecl* fieldDecl);

	bool VisitEnumDecl(clang::EnumDecl* enumDecl);

private:
	clang::ASTContext* m_context;

	// This is what the visitor functions need to fill
	IRProxy::IRData m_irData;

	// This will get filled in the destructor of the visitor
	IR::Namespace& m_parsedNamespaces;

	// Set to false if any visitor found an unrecoverable error
	bool& m_parsedSuccessfully;

	bool isInSystemHeader(clang::Decl* decl) {
		return m_context->getSourceManager().isInSystemHeader(
		    decl->getLocation());
	}
};
}    // namespace Visitor
