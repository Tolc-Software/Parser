#include "Parser/Parse.hpp"
#include "Factory/ParserFrontendActionFactory.hpp"
#include "Frontend/ParserFrontendAction.hpp"
#include "Helpers/Utils/combine.hpp"
#include "Helpers/commandLineArgs.hpp"
#include "Parser/Config.hpp"
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/Tooling.h>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace Parser {
std::optional<IR::Namespace> parseFile(std::filesystem::path const& filename,
                                       Parser::Config const& config) {
	IR::Namespace parsedIR;

	// Create the db for flags
	std::string fromDirectory = ".";

	clang::tooling::FixedCompilationDatabase compDb(
	    fromDirectory, Helpers::getCommandLineArgs(config.m_systemIncludes));

	clang::tooling::ClangTool tool(compDb, {filename});

	auto parsedSuccessfully =
	    tool.run(Factory::newParserFrontendActionFactory(parsedIR).get()) == 0;

	if (parsedSuccessfully) {
		return parsedIR;
	}
	return std::nullopt;
}

std::optional<IR::Namespace> parseString(std::string const& code,
                                         Parser::Config const& config) {
	IR::Namespace parsedIR;

	auto parsedSuccessfully = clang::tooling::runToolOnCodeWithArgs(
	    std::make_unique<Frontend::ParserFrontendAction>(parsedIR),
	    code.c_str(),
	    Helpers::getCommandLineArgs(config.m_systemIncludes));

	if (parsedSuccessfully) {
		return parsedIR;
	}
	return std::nullopt;
}

}    // namespace Parser
