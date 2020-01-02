#pragma once

#include "Helpers/IRData.h"
#include "Visitor/ParserVisitor.h"
#include "clang/AST/ASTConsumer.h"

namespace Consumer {

class ParserConsumer : public clang::ASTConsumer {
public:
	explicit ParserConsumer(clang::ASTContext* context, Helpers::IRData& irData)
	    : m_visitor(context, irData) {}

	virtual void HandleTranslationUnit(clang::ASTContext& context);

private:
	Visitor::ParserVisitor m_visitor;
};

}
