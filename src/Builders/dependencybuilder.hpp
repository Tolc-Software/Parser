#pragma once

#include "Parser/MetaData.hpp"
#include <IR/ir.hpp>
#include <map>
#include <vector>

namespace Builders {

void buildDependency(IR::Namespace const& ns,
                     std::map<std::string, size_t> const& idMap,
                     std::vector<std::vector<size_t>>& dependencies);

void createDefinitionOrder(
    Parser::MetaData& metaData,
    std::vector<std::vector<size_t>>& dependencies);
}    // namespace Builders
