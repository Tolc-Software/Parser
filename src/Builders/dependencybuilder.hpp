#pragma once

#include <IR/ir.hpp>
#include <map>
#include <optional>
#include <set>
#include <vector>

namespace Builders {

/*
* Adds to dependencies the links between id => depends on ids
*/
void buildDependency(IR::Namespace const& ns,
                     std::map<std::string, size_t> const& idMap,
                     std::vector<std::set<size_t>>& dependencies);

/*
* From dependencies, create the order in which the objects can be defined
* This is not unique.
*/
std::optional<std::vector<size_t>>
createDefinitionOrder(std::vector<std::set<size_t>> const& dependencies);
}    // namespace Builders
