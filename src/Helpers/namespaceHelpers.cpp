#include "Helpers/namespaceHelpers.h"
#include "IR/ir.hpp"
#include <algorithm>
#include <vector>

namespace Helpers {

std::vector<IR::Namespace>::iterator
findNamespace(std::vector<IR::Namespace>& namespaces, std::string_view name) {
	return std::find_if(namespaces.begin(),
	                    namespaces.end(),
	                    [name](auto const& n) { return n.m_name == name; });
}
}
