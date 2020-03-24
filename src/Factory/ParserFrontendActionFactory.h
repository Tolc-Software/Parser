#pragma once
#include "Frontend/ParserFrontendAction.h"
#include <IR/ir.hpp>
#include <clang/Tooling/Tooling.h>
#include <memory>

namespace Factory {
std::unique_ptr<clang::tooling::FrontendActionFactory>
newParserFrontendActionFactory(IR::Namespace& ns) {
	class ParserFrontendActionFactory
	    : public clang::tooling::FrontendActionFactory {
	public:
		ParserFrontendActionFactory(IR::Namespace& parsedNamespaces)
		    : m_parsedNamespaces(parsedNamespaces) {}
		IR::Namespace& m_parsedNamespaces;
		clang::FrontendAction* create() override {
			return new Frontend::ParserFrontendAction(m_parsedNamespaces);
		}
	};

	return std::unique_ptr<clang::tooling::FrontendActionFactory>(
	    new ParserFrontendActionFactory(ns));
}
}    // namespace Factory
