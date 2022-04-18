#pragma once

#include "IR/ir.hpp"
#include "Parser/Config.hpp"
#include "Parser/MetaData.hpp"
#include <filesystem>
#include <string>

namespace Parser {
/**
* @brief: Parse the input file as C++ code and return the intermediate representation.
*
* @param: std::filesystem::path const& filename
*
* @return: IR::Namespace - The global namespace
*          Parser::Metadata - Data about the parsed code
*/
std::optional<std::pair<IR::Namespace, Parser::MetaData>>
parseFile(std::filesystem::path const& filename,
          Parser::Config const& config = Parser::Config());

/**
* @brief: Parse the input as C++ code and return the intermediate representation.
*
* @param: std::string const& code
*
* @return: IR::Namespace - The global namespace
*          Parser::Metadata - Data about the parsed code
*/
std::optional<std::pair<IR::Namespace, Parser::MetaData>>
parseString(std::string const& code,
            Parser::Config const& config = Parser::Config());
}
