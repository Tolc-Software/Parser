#include "Builders/dependencybuilder.hpp"
#include <IR/ir.hpp>
#include <algorithm>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <vector>

#include <fmt/format.h>

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
// Helper class to have a queue that knows its items
struct DependencyQueue {
	bool empty() {
		return ids.empty();
	}

	size_t front() {
		return ids.front();
	}

	void push(size_t id) {
		ids.push(id);
		inQueue.insert(id);
	}

	void pop(size_t id) {
		ids.pop();
		inQueue.erase(id);
	}

	bool contains(size_t id) {
		return inQueue.contains(id);
	}

	std::queue<size_t> ids;
	std::set<size_t> inQueue;
};

std::pair<DependencyQueue, std::vector<std::vector<size_t>>>
initializeDependencyData(std::vector<std::set<size_t>> const& dependencyMap) {
	// [id: {dependent on id}]
	std::vector<std::vector<size_t>> reversed(dependencyMap.size(),
	                                          std::vector<size_t>());
	DependencyQueue noDependencies;
	size_t id = 0;
	for (auto const& dependencies : dependencyMap) {
		for (auto const& dependency : dependencies) {
			reversed[dependency].push_back(id);
		}
		if (dependencies.empty()) {
			noDependencies.push(id);
		}
		id++;
	}
	return {noDependencies, reversed};
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

void addPotentiallySolvedToQueue(std::vector<size_t> const& potentiallySolved,
                                 std::set<size_t> const& definedPreviously,
                                 DependencyQueue& ids) {
	// The now potentially solved ids
	for (auto const& maybeSolved : potentiallySolved) {
		if (!definedPreviously.contains(maybeSolved) &&
		    !ids.contains(maybeSolved)) {
			ids.push(maybeSolved);
		}
	}
}

std::optional<std::vector<size_t>>
createDefinitionOrder(std::vector<std::set<size_t>> const& dependencyMap) {
	// The ones already put into the ordered list of objects to define
	std::set<size_t> defined;
	std::vector<size_t> order;

	// Start with the ids that have no dependencies
	auto [idsToProcess, reversedDependencyMap] =
	    initializeDependencyData(dependencyMap);

	while (!idsToProcess.empty()) {
		// Has this round put us closer to a solution?
		bool hasProgressed = false;

		size_t id = idsToProcess.front();
		auto const& dependencies = dependencyMap[id];

		if (dependencies.empty() && defined.insert(id).second) {
			// No dependencies
			// Never added before => Add it
			order.push_back(id);
			hasProgressed = true;

			// The ones who had id as a dependency
			addPotentiallySolvedToQueue(
			    reversedDependencyMap[id], defined, idsToProcess);
		} else {
			if (std::all_of(dependencies.begin(),
			                dependencies.end(),
			                [&defined](auto dependency) {
				                return defined.contains(dependency);
			                })) {
				defined.insert(id);

				// All of the dependencies are defined
				// => we can add it
				order.push_back(id);
				hasProgressed = true;

				// The ones who had id as a dependency
				addPotentiallySolvedToQueue(
				    reversedDependencyMap[id], defined, idsToProcess);
			}
		}
		idsToProcess.pop(id);

		if (!hasProgressed) {
			// No progress made => There must be a circular dependency
			return std::nullopt;
		}
	}

	// Could have been no objects with no dependencies
	if (order.size() != dependencyMap.size()) {
		return std::nullopt;
	}
	return order;
}
}    // namespace Builders
