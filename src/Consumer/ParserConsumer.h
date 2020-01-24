#pragma once

#include "IR/ir.hpp"
#include "Visitor/ParserVisitor.h"
#include "clang/AST/ASTConsumer.h"

namespace Consumer {

class ParserConsumer : public clang::ASTConsumer {
public:
	explicit ParserConsumer(clang::ASTContext* context,
	                        IR::Namespace& parsedNamespaces)
	    : m_visitor(context, parsedNamespaces) {}

	virtual void HandleTranslationUnit(clang::ASTContext& context);

private:
	Visitor::ParserVisitor m_visitor;
};

}
