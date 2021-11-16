#include "Builders/commonBuilder.hpp"
#include "Builders/fieldBuilder.hpp"
#include "Builders/functionBuilder.hpp"
#include "Builders/typeBuilder.hpp"
#include "IRProxy/IRData.hpp"
#include "Visitor/ParserVisitor.hpp"
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

namespace {

struct TemplateHelper {
	//     {Type name, Index in templateArgs}
	// Ex: {"int",     0}
	std::map<std::string, std::size_t> templateToTypeIndex;
	//     {Actual clang types}
	// Ex: {int}
	std::vector<clang::QualType> templateArgs;
};

// Returns the exchanged templated type if defaultType is a template parameter (say "T")
// Returns the defaultType if it is a normal type
clang::QualType getTemplatedType(TemplateHelper const& helper,
                                 clang::QualType defaultType) {
	// Is this a templated type?
	if (auto typeIndex =
	        helper.templateToTypeIndex.find(defaultType.getAsString());
	    typeIndex != helper.templateToTypeIndex.end()) {
		defaultType = helper.templateArgs[typeIndex->second];
	}
	return defaultType;
}

void adjustWithTemplateParameters(IRProxy::Function& f,
                                  IRProxy::Struct const& s) {
	// Adjust the path with the templated parent
	auto functionName = f.m_path.back();
	auto funcPath = s.m_path;
	funcPath.push_back(functionName);
	f.m_path = funcPath;

	// Adjust the fully qualified name
	f.m_fullyQualifiedName = s.m_fullyQualifiedName + "::" + functionName.first;
}

void adjustConstructorName(IRProxy::Function& f, IRProxy::Struct const& s) {
	// MyClass<int>
	auto classInstantiation = s.m_path.back().first;

	// Adjust the path with the templated parent
	f.m_path.back().first = classInstantiation;

	// Adjust the fully qualified name
	f.m_fullyQualifiedName = s.m_fullyQualifiedName + "::" + classInstantiation;
}
}    // namespace

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
	TemplateHelper helper;
	std::vector<std::string> templateStringArgs;
	for (auto const& templateArg : classDecl->getTemplateArgs().asArray()) {
		auto arg = templateArg.getAsType();
		helper.templateArgs.push_back(arg);
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

	if (auto classTemplate =
	        m_templateCache.findStruct(classDecl->getNameAsString())) {
		parsedStruct.m_hasImplicitDefaultConstructor =
		    classTemplate->hasDefaultConstructor();

		std::size_t i = 0;
		for (auto const& par : classTemplate->getDescribedClassTemplate()
		                           ->getTemplateParameters()
		                           ->asArray()) {
			helper.templateToTypeIndex[par->getNameAsString()] = i;
			i++;
		}

		for (auto const& memberVariable : classTemplate->fields()) {
			auto memberType =
			    getTemplatedType(helper, memberVariable->getType());
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

		for (auto const& method : classTemplate->methods()) {
			auto [status, parsedFunc] =
			    Builders::buildFunction(method, [&helper](auto type) {
				    return getTemplatedType(helper, type);
			    });
			switch (status) {
				case (Builders::FunctionError::Ok): {
					adjustWithTemplateParameters(parsedFunc.value(),
					                             parsedStruct);
					if (llvm::isa<clang::CXXConstructorDecl>(method)) {
						adjustConstructorName(parsedFunc.value(), parsedStruct);
					}
					m_irData.m_functions.push_back(parsedFunc.value());
					break;
				}
				case (Builders::FunctionError::ArgumentType): {
					spdlog::error(
					    R"(Failed to parse argument type for function "{}")",
					    method->getQualifiedNameAsString());
					m_parsedSuccessfully = false;
					// Stop parsing
					return false;
				}
				case (Builders::FunctionError::ReturnType): {
					spdlog::error(
					    R"(Failed to parse return type for function "{}")",
					    method->getQualifiedNameAsString());
					m_parsedSuccessfully = false;
					// Stop parsing
					return false;
				}
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