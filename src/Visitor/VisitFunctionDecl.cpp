#include "Builders/commonBuilder.hpp"
#include "Builders/functionBuilder.hpp"
#include "Builders/typeBuilder.hpp"
#include "Visitor/ParserVisitor.hpp"
#include <spdlog/spdlog.h>

namespace {
void adjustWithTemplateParams(IRProxy::Function& f,
                              clang::FunctionDecl* fDecl) {
	// Potentially add the template parameters to name/representation
	std::vector<std::string> params;
	if (auto templateArgs = fDecl->getTemplateSpecializationArgs()) {
		for (auto param : templateArgs->asArray()) {
			params.push_back(param.getAsType().getAsString());
		}
	}
	if (!params.empty()) {
		// <int, double>
		std::string paramPack = fmt::format("<{}>", fmt::join(params, ", "));
		f.m_fullyQualifiedName += paramPack;
		f.m_path.back().first += paramPack;
	}
}
}    // namespace

namespace Visitor {
bool ParserVisitor::VisitFunctionDecl(clang::FunctionDecl* functionDecl) {
	if (isInSystemHeader(functionDecl) || isPureTemplate(functionDecl)) {
		// Continue the AST search
		return true;
	}
	spdlog::debug(R"(Parsing function: "{}")",
	              functionDecl->getQualifiedNameAsString());
	auto [status, parsedFunc] = Builders::buildFunction(functionDecl);
	switch (status) {
		case (Builders::FunctionError::Ok):
			adjustWithTemplateParams(parsedFunc.value(), functionDecl);

			m_irData.m_functions.push_back(parsedFunc.value());
			break;
		case (Builders::FunctionError::ArgumentType):
			spdlog::error(R"(Failed to parse argument type for function "{}")",
			              functionDecl->getQualifiedNameAsString());
			m_parsedSuccessfully = false;
			// Stop parsing
			return false;
			break;
		case (Builders::FunctionError::ReturnType):
			spdlog::error(R"(Failed to parse return type for function "{}")",
			              functionDecl->getQualifiedNameAsString());
			m_parsedSuccessfully = false;
			// Stop parsing
			return false;
			break;
	}

	// Continue the AST search
	return true;
}

}    // namespace Visitor
