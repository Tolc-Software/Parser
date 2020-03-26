#include "Parser/Parse.h"
#include "Factory/ParserFrontendActionFactory.h"
#include "Frontend/ParserFrontendAction.h"
#include "Helpers/includes.h"
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/Tooling.h>
#include <memory>
#include <string>

namespace Parser {
IR::Namespace parseFile(std::string const& filename) {
	IR::Namespace parsedIR;

	// Create the db for flags
	std::string fromDirectory = ".";
	clang::tooling::FixedCompilationDatabase compDb(
	    fromDirectory, Helpers::getSystemIncludes());

	clang::tooling::ClangTool tool(compDb, {filename});
	tool.run(Factory::newParserFrontendActionFactory(parsedIR).get());

	return parsedIR;
}

IR::Namespace parseString(std::string const& code) {
	IR::Namespace parsedIR;
	clang::tooling::runToolOnCodeWithArgs(
	    std::make_unique<Frontend::ParserFrontendAction>(parsedIR),
	    code.c_str(),
	    Helpers::getSystemIncludes());

	return parsedIR;
}

}    // namespace Parser
