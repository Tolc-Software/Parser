#pragma once

#include <clang/AST/DeclCXX.h>
#include <string>
#include <vector>

namespace Template {
struct Cache {
	// Returns nullptr if not found
	clang::CXXRecordDecl* findStruct(std::string const& strct);

	// Uninstantiated classes/structs that are templated.
	// Lives until the end of parsing the clang AST
	// Ex:
	//   template <typename T>
	//   class MyClass {
	//     ...
	//   };
	std::vector<clang::CXXRecordDecl*> m_structs;
};
}    // namespace Template