#pragma once

#include <IR/ir.hpp>
#include <map>
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
* Returns true on success
*/
bool createDefinitionOrder(std::vector<std::set<size_t>> const& dependencies,
                           std::vector<size_t>& definitionOrder);
}    // namespace Builders
