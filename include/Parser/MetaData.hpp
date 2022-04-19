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
	// Contains the order that objects need to be defined in
	// For instance:
	//   struct A {}
	// Need to be defined before:
	//   A a;
	// {id}
	std::vector<size_t> m_definitionOrder {};
};
}    // namespace Parser
