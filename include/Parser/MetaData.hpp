#pragma once

#include <vector>

namespace Parser {
/**
* The values in this struct corresponds to data
* related to the code the Parser just went through
* - There should be no functions or logic here, just values.
* - Every top level value should be commented.
*/
struct MetaData {
	// {Object: {Objects which depend on key}}
	// {id: {ids}}
	std::vector<std::vector<size_t>> m_dependencyMap {};
};
}    // namespace Parser
