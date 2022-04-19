#include "Builders/dependencybuilder.hpp"
#include <IR/ir.hpp>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>

namespace Builders {

namespace {

void copyDependenciesFromTo(size_t from,
                            size_t to,
                            std::vector<std::set<size_t>>& dependencies) {
	for (auto dep : dependencies[from]) {
		// Don't depend on yourself
		if (dep != to) {
			dependencies[to].insert(dep);
		}
	}
}

std::optional<size_t>
getDependencyFromMap(std::string const& key,
                     std::map<std::string, size_t> const& m) {
	if (auto v = m.find(key); v != m.end()) {
		return v->second;
	}
	return std::nullopt;
}

std::optional<size_t>
getDependencyFromType(IR::Type const& type,
                      std::map<std::string, size_t> const& idMap) {
	if (auto enumType = std::get_if<IR::Type::EnumValue>(&type.m_type)) {
		return getDependencyFromMap(enumType->m_representation, idMap);
	} else if (auto userDefined =
	               std::get_if<IR::Type::UserDefined>(&type.m_type)) {
		return getDependencyFromMap(userDefined->m_representation, idMap);
	}

	return std::nullopt;
}

void addVariableDependencies(IR::Variable const& variable,
                             std::map<std::string, size_t> const& idMap,
                             std::vector<std::set<size_t>>& dependencies) {
	if (auto dep = getDependencyFromType(variable.m_type, idMap)) {
		dependencies[variable.m_id].insert(dep.value());
	}
}

void addArgumentDependencies(IR::Argument const& arg,
                             size_t functionId,
                             std::map<std::string, size_t> const& idMap,
                             std::vector<std::set<size_t>>& dependencies) {
	if (auto dep = getDependencyFromType(arg.m_type, idMap)) {
		dependencies[functionId].insert(dep.value());
	}
}

void addTemplateDependencies(size_t parentId,
                             std::vector<IR::Type> const& templateArgs,
                             std::map<std::string, size_t> const& idMap,
                             std::vector<std::set<size_t>>& dependencies) {
	for (auto const& arg : templateArgs) {
		if (auto dep = getDependencyFromType(arg, idMap)) {
			dependencies[parentId].insert(dep.value());
		}
	}
}

void addFunctionArgumentDependencies(
    IR::Function const& f,
    std::map<std::string, size_t> const& idMap,
    std::vector<std::set<size_t>>& dependencies) {
	for (auto const& arg : f.m_arguments) {
		addArgumentDependencies(arg, f.m_id, idMap, dependencies);
	}

	addTemplateDependencies(f.m_id, f.m_templateArguments, idMap, dependencies);
}

void addFunctionDependencies(IR::Function const& f,
                             std::map<std::string, size_t> const& idMap,
                             std::vector<std::set<size_t>>& dependencies) {
	if (auto dep = getDependencyFromType(f.m_returnType, idMap)) {
		dependencies[f.m_id].insert(dep.value());
	}

	addFunctionArgumentDependencies(f, idMap, dependencies);
}

void addStructDataDependencies(IR::StructData const& data,
                               size_t parentId,
                               std::map<std::string, size_t> const& idMap,
                               std::vector<std::set<size_t>>& dependencies) {
	for (auto const& c : data.m_constructors) {
		addFunctionArgumentDependencies(c, idMap, dependencies);
		// The constructors dependencies are the structs as well
		copyDependenciesFromTo(c.m_id, parentId, dependencies);
		// All of these depend on the struct being defined
		dependencies[c.m_id].insert(parentId);
	}

	for (auto const& d : data.m_destructors) {
		addFunctionArgumentDependencies(d, idMap, dependencies);
		// The destructors dependencies are the structs as well
		copyDependenciesFromTo(d.m_id, parentId, dependencies);
		// All of these depend on the struct being defined
		dependencies[d.m_id].insert(parentId);
	}

	for (auto const& f : data.m_functions) {
		addFunctionDependencies(f, idMap, dependencies);
		// The member functions dependencies are the structs as well
		copyDependenciesFromTo(f.m_id, parentId, dependencies);
		// All of these depend on the struct being defined
		dependencies[f.m_id].insert(parentId);
	}

	for (auto const& e : data.m_enums) {
		// The nested enum depends on the class
		dependencies[e.m_id].insert(parentId);
		// All of these depend on the struct being defined
		dependencies[e.m_id].insert(parentId);
	}

	for (auto const& variable : data.m_memberVariables) {
		addVariableDependencies(variable, idMap, dependencies);
		copyDependenciesFromTo(variable.m_id, parentId, dependencies);
		// All of these depend on the struct being defined
		dependencies[variable.m_id].insert(parentId);
	}

	for (auto const& op : data.m_operators) {
		addFunctionArgumentDependencies(op.second, idMap, dependencies);
		// The destructors dependencies are the structs as well
		copyDependenciesFromTo(op.second.m_id, parentId, dependencies);
		// All of these depend on the struct being defined
		dependencies[op.second.m_id].insert(parentId);
	}

	for (auto const& i : data.m_inherited) {
		// This class depends on the inherited classes
		if (auto ids = idMap.find(i); ids != idMap.end()) {
			copyDependenciesFromTo(ids->second, parentId, dependencies);
		}
	}

	// TODO: Nested structs might create more dependencies
}

void addStructDependencies(IR::Struct const& s,
                           std::map<std::string, size_t> const& idMap,
                           std::vector<std::set<size_t>>& dependencies) {
	addStructDataDependencies(s.m_public, s.m_id, idMap, dependencies);
	addStructDataDependencies(s.m_private, s.m_id, idMap, dependencies);
	addStructDataDependencies(s.m_protected, s.m_id, idMap, dependencies);

	addTemplateDependencies(s.m_id, s.m_templateArguments, idMap, dependencies);
}

void addNamespaceDependencies(IR::Namespace const& ns,
                              std::map<std::string, size_t> const& idMap,
                              std::vector<std::set<size_t>>& dependencies) {
	for (auto const& variable : ns.m_variables) {
		addVariableDependencies(variable, idMap, dependencies);
	}

	for (auto const& f : ns.m_functions) {
		addFunctionDependencies(f, idMap, dependencies);
	}

	for (auto const& s : ns.m_structs) {
		addStructDependencies(s, idMap, dependencies);
	}
}
}    // namespace

void buildDependency(IR::Namespace const& ns,
                     std::map<std::string, size_t> const& idMap,
                     std::vector<std::set<size_t>>& dependencies) {
	std::queue<IR::Namespace const*> nsToProcess;
	nsToProcess.push(&ns);

	while (!nsToProcess.empty()) {
		auto const* currentNs = nsToProcess.front();

		addNamespaceDependencies(*currentNs, idMap, dependencies);

		// Next level of namespaces
		for (auto const& n : currentNs->m_namespaces) {
			nsToProcess.push(&n);
		}

		nsToProcess.pop();
	}
}

bool createDefinitionOrder(std::vector<std::set<size_t>> const& dependencyMap,
                           std::vector<size_t>& definitionOrder) {
	// The ones already put into the ordered list of objects to define
	std::set<size_t> alreadyOrdered;

	bool hasProgressed = true;
	while (definitionOrder.size() != dependencyMap.size()) {
		// Has this round put us closer to a solution?
		hasProgressed = false;

		size_t index = 0;
		for (auto const& dependencies : dependencyMap) {
			if (dependencies.empty()) {
				// No dependencies => we can add it
				if (auto hasAdded = alreadyOrdered.insert(index);
				    hasAdded.second) {
					// Never added before => Add it
					definitionOrder.push_back(index);
					hasProgressed = true;
				}
			} else {
				// Has dependencies => Have we defined all of them before?
				bool canAdd = true;
				for (auto dependency : dependencies) {
					if (alreadyOrdered.find(dependency) ==
					    alreadyOrdered.end()) {
						// Found a dependency not previously defined => Stop for loop
						canAdd = false;
						break;
					}
				}
				if (canAdd) {
					// No children => we can add it
					definitionOrder.push_back(index);
					alreadyOrdered.insert(index);
					hasProgressed = true;
				}
			}
			index++;
		}

		if (!hasProgressed) {
			// No progress made => There must be a circular dependency
			return false;
		}
	}
	return true;
}
}    // namespace Builders
