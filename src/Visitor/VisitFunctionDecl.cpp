#include "Helpers/Builders/commonBuilder.h"
#include "Helpers/Utils/split.h"
#include "Visitor/ParserVisitor.h"
#include "clang/AST/Type.h"

#include <iostream>
#include <string>

namespace Visitor {

bool ParserVisitor::VisitFunctionDecl(clang::FunctionDecl* functionDecl) {
	std::cout << "I'm called with function: "
	          << functionDecl->getQualifiedNameAsString() << '\n';
	auto splitNames =
	    Helpers::Utils::split(functionDecl->getQualifiedNameAsString(), "::");

	// Only interested in the structure from here on up
	// We know this is a record
	auto nameOfFunction = splitNames.back();
	splitNames.pop_back();
	auto structure = Helpers::Builders::buildParentStructure(
	    functionDecl->getParent(), splitNames);
	// Push the function back in
	structure.push_back({nameOfFunction, IRProxy::Type::Function});

	IRProxy::Function parsedFunc;
	parsedFunc.m_name = structure;

	if (auto returnType = Helpers::Builders::getType(
	        functionDecl->getReturnType().getAsString())) {
		parsedFunc.m_returnType = returnType.value();
	}

	m_irData.m_functions.push_back(parsedFunc);

	for (auto& p : functionDecl->parameters()) {
		std::cout << "===== Start of variable =====" << '\n';
		std::cout << "Name: " << std::string(p->getName()) << '\n';
		std::cout << "All of the type: "
		          << clang::QualType::getAsString(p->getType().split(),
		                                          clang::PrintingPolicy {{}})
		          << '\n';
		std::cout << "Qualifiers: " << p->getType().split().Quals.getAsString()
		          << '\n';
		std::cout << "Has const: " << p->getType().split().Quals.hasConst()
		          << '\n';

		p->getType().split().Ty->dump();
		p->getType()->dump();
		p->dump();

		clang::QualifierCollector qc;
		auto withoutQs = qc.strip(p->getType());
		withoutQs->dump();
		std::cout << "===== End of variable =====" << '\n';
		std::cout << '\n';
	}
	// Continue the AST search
	return true;
}

}
