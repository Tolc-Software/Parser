#pragma once

#include "IR/ir.hpp"
#include <vector>

namespace Helpers {

struct IRData {
	std::vector<IR::Namespace> m_namespaces;
	std::vector<IR::Function> m_functions;
};
}
