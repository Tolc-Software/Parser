#include "Visitor/Helpers/getDocumentation.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RawCommentList.h>
#include <clang/Basic/CommentOptions.h>
#include <clang/Basic/Diagnostic.h>
#include <iostream>
#include <string>

namespace Visitor::Helpers {

std::string getDocumentation(clang::Decl* decl) {
	std::string documentation;
	clang::ASTContext& ctx = decl->getASTContext();
	clang::SourceManager& sm = ctx.getSourceManager();

	const auto* rawComment = ctx.getRawCommentForDeclNoCache(decl);
	if (rawComment) {
		auto raw = rawComment->getRawText(sm);
		std::cout << "Raw:" << '\n';
		std::cout << std::string(raw) << '\n';
		clang::DiagnosticsEngine diags(new clang::DiagnosticIDs,
		                               new clang::DiagnosticOptions);
		documentation = rawComment->getFormattedText(sm, diags);
		std::cout << "Documentation: " << '\n';
		std::cout << documentation << '\n';
		std::cout << "=====================" << '\n';
	} else {
		std::cout << "Couldn't find documentation for decl: "
		          << decl->getDeclKindName() << '\n';
	}
	return documentation;
}
}    // namespace Visitor::Helpers
