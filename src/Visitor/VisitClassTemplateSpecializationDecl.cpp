#include "Builders/commonBuilder.hpp"
#include "Builders/fieldBuilder.hpp"
#include "Builders/typeBuilder.hpp"
#include "IRProxy/IRData.hpp"
#include "Visitor/ParserVisitor.hpp"
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

namespace Visitor {

bool ParserVisitor::VisitClassTemplateSpecializationDecl(
    clang::ClassTemplateSpecializationDecl* classDecl) {
	if (isInSystemHeader(classDecl)) {
		// Continue the AST search
		return true;
	}

	// Build the final parsedStruct
	IRProxy::Struct parsedStruct;

	parsedStruct.m_fullyQualifiedName = classDecl->getQualifiedNameAsString();
	std::vector<clang::QualType> templateArgs;
	std::vector<std::string> templateStringArgs;
	for (auto const& templateArg : classDecl->getTemplateArgs().asArray()) {
		auto arg = templateArg.getAsType();
		templateArgs.push_back(arg);
		templateStringArgs.push_back(arg.getAsString());
	}
	auto templateParameters =
	    fmt::format("<{}>", fmt::join(templateStringArgs, ", "));
	parsedStruct.m_fullyQualifiedName += templateParameters;

	parsedStruct.m_path =
	    Builders::buildStructure(classDecl, IRProxy::Structure::Struct);
	parsedStruct.m_path.back().first += templateParameters;

	spdlog::debug(R"(Parsing template instantiated class/struct: "{}")",
	              parsedStruct.m_fullyQualifiedName);

	// TODO: Find out how to do this
	parsedStruct.m_hasImplicitDefaultConstructor = true;

	if (auto classTemplate =
	        m_templateCache.findStruct(classDecl->getNameAsString())) {
		// {Template parameter name, index for specialization found in templateArgs}
		std::map<std::string, std::size_t> templateToTypeIndex;
		std::size_t i = 0;
		for (auto const& par : classTemplate->getDescribedClassTemplate()
		                           ->getTemplateParameters()
		                           ->asArray()) {
			templateToTypeIndex[par->getNameAsString()] = i;
			i++;
		}

		for (auto const& memberVariable : classTemplate->fields()) {
			auto memberType = memberVariable->getType();
			// Is this a templated type?
			if (auto typeIndex =
			        templateToTypeIndex.find(memberType.getAsString());
			    typeIndex != templateToTypeIndex.end()) {
				memberType = templateArgs[typeIndex->second];
			}
			if (auto maybeField =
			        Builders::buildField(memberVariable, memberType)) {
				auto [access, variable] = maybeField.value();
				parsedStruct.m_variables.push_back({access, variable});
			} else {
				spdlog::error(R"(Failed to parse member variable "{}::{}")",
				              parsedStruct.m_fullyQualifiedName,
				              memberVariable->getQualifiedNameAsString());
				m_parsedSuccessfully = false;
				// Stop parsing
				return false;
			}
		}
	} else {
		spdlog::error(
		    R"(Failed to find template declaration for template specialization "{}". Did it appear before the specialization in the source?)",
		    parsedStruct.m_fullyQualifiedName);
		m_parsedSuccessfully = false;
		// Stop parsing
		return false;
	}

	m_irData.m_structs.push_back(parsedStruct);
	// Continue the AST search
	return true;
}

}    // namespace Visitor
