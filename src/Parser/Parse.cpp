#include "Parser/Parse.hpp"
#include "Factory/ParserFrontendActionFactory.hpp"
#include "Frontend/ParserFrontendAction.hpp"
#include "Helpers/Utils/combine.hpp"
#include "Helpers/commandLineArgs.hpp"
#include "Helpers/logging.hpp"
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
	Helpers::setupLogging(config.m_logLevel);

	// Create the db for flags
	std::string fromDirectory = ".";

	clang::tooling::FixedCompilationDatabase compDb(
	    fromDirectory, Helpers::getCommandLineArgs(config.m_systemIncludes));

	clang::tooling::ClangTool tool(compDb, {filename});

	bool parsedSuccessfully = true;

	IR::Namespace parsedIR;
	auto astCreated = tool.run(Factory::newParserFrontendActionFactory(
	                               parsedIR, parsedSuccessfully)
	                               .get()) == 0;

	if (astCreated && parsedSuccessfully) {
		return parsedIR;
	}
	return std::nullopt;
}

std::optional<IR::Namespace> parseString(std::string const& code,
                                         Parser::Config const& config) {
	Helpers::setupLogging(config.m_logLevel);

	bool parsedSuccessfully = true;
	IR::Namespace parsedIR;

	auto astCreated = clang::tooling::runToolOnCodeWithArgs(
	    std::make_unique<Frontend::ParserFrontendAction>(parsedIR,
	                                                     parsedSuccessfully),
	    code,
	    Helpers::getCommandLineArgs(config.m_systemIncludes));

	if (astCreated && parsedSuccessfully) {
		return parsedIR;
	}
	return std::nullopt;
}

}    // namespace Parser
