#pragma once

#include "Visitor/ParserVisitor.h"
#include "clang/AST/ASTConsumer.h"

namespace Consumer {

class ParserConsumer : public clang::ASTConsumer {
public:
	explicit ParserConsumer(clang::ASTContext* context) : m_visitor(context) {}

	virtual void HandleTranslationUnit(clang::ASTContext& context);

private:
	Visitor::ParserVisitor m_visitor;
};

}
