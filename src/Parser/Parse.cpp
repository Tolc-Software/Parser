#include "Parser/Parse.hpp"
#include "Factory/ParserFrontendActionFactory.hpp"
#include "Frontend/ParserFrontendAction.hpp"
#include "Helpers/Utils/combine.hpp"
#include "Helpers/commandLineArgs.hpp"
#include "Parser/Config.hpp"
#include "Parser/MetaData.hpp"
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/Tooling.h>
#include <filesystem>
#include <llvm/ADT/ArrayRef.h>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace Parser {
std::optional<std::pair<IR::Namespace, Parser::MetaData>>
parseFile(std::filesystem::path const& filename, Parser::Config const& config) {
	// Create the db for flags
	std::string fromDirectory = ".";

	clang::tooling::FixedCompilationDatabase compDb(
	    fromDirectory, Helpers::getCommandLineArgs(config.m_systemIncludes));

	std::vector<std::string> filesToProcess = {filename.string()};
	clang::tooling::ClangTool tool(compDb, filesToProcess);

	bool parsedSuccessfully = true;

	IR::Namespace parsedIR;
	Parser::MetaData metaData;
	auto astCreated = tool.run(Factory::newParserFrontendActionFactory(
	                               parsedIR, metaData, parsedSuccessfully)
	                               .get()) == 0;

	if (astCreated && parsedSuccessfully) {
		return std::make_pair(parsedIR, metaData);
	}
	return std::nullopt;
}

std::optional<std::pair<IR::Namespace, Parser::MetaData>>
parseString(std::string const& code, Parser::Config const& config) {
	bool parsedSuccessfully = true;
	IR::Namespace parsedIR;
	Parser::MetaData metaData;

	auto astCreated = clang::tooling::runToolOnCodeWithArgs(
	    std::make_unique<Frontend::ParserFrontendAction>(
	        parsedIR, metaData, parsedSuccessfully),
	    code,
	    Helpers::getCommandLineArgs(config.m_systemIncludes));

	if (astCreated && parsedSuccessfully) {
		return std::make_pair(parsedIR, metaData);
	}
	return std::nullopt;
}

}    // namespace Parser
