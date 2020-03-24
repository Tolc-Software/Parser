#include "Parser/Parse.h"
#include "Frontend/ParserFrontendAction.h"
#include "Helpers/includes.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Tooling/Tooling.h"
#include <string>

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

namespace Parser {
IR::Namespace parseFile(std::string const& filename) {
	IR::Namespace parsedIR;

	// Create the db for flags
	std::string fromDirectory = ".";
	clang::tooling::FixedCompilationDatabase compDb(
	    fromDirectory, Helpers::getSystemIncludes());

	clang::tooling::ClangTool tool(compDb, {filename});
	// TODO: Create a new factory by inheriting from newFrontendActionFactory
	tool.run(newParserFrontendActionFactory(parsedIR).get());
	return parsedIR;
}

IR::Namespace parseString(std::string const& code) {
	IR::Namespace parsedIR;
	clang::tooling::runToolOnCodeWithArgs(
	    new Frontend::ParserFrontendAction(parsedIR),
	    code.c_str(),
	    Helpers::getSystemIncludes());

	return parsedIR;
}

}    // namespace Parser
