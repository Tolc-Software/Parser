#include "Parser/Parse.hpp"
#include "Factory/ParserFrontendActionFactory.hpp"
#include "Frontend/ParserFrontendAction.hpp"
#include "Helpers/includes.hpp"
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/Tooling.h>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>

namespace Parser {
std::optional<IR::Namespace> parseFile(std::filesystem::path const& filename) {
	IR::Namespace parsedIR;

	// Create the db for flags
	std::string fromDirectory = ".";

	clang::tooling::FixedCompilationDatabase compDb(
	    fromDirectory, Helpers::getSystemIncludes());

	clang::tooling::ClangTool tool(compDb, {filename});

	auto error =
	    tool.run(Factory::newParserFrontendActionFactory(parsedIR).get());

	if (error == 0) {
		return parsedIR;
	}
	return std::nullopt;
}

std::optional<IR::Namespace> parseString(std::string const& code) {
	IR::Namespace parsedIR;

	auto parsedSuccessfully = clang::tooling::runToolOnCodeWithArgs(
	    std::make_unique<Frontend::ParserFrontendAction>(parsedIR),
	    code.c_str(),
	    Helpers::getSystemIncludes());

	if (parsedSuccessfully) {
		return parsedIR;
	}
	return std::nullopt;
}

}    // namespace Parser
