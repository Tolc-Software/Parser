#pragma once

#include "IRProxy/IRData.hpp"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>
#include <string>

namespace TestUtil {
std::pair<std::string, std::string>
getNameAndRepresentation(IRProxy::Structure const& structure,
                         IR::Struct const& s) {
	using IRProxy::Structure;
	switch (structure) {
		case Structure::Struct:
			REQUIRE(s.m_structs.size() == 1);
			return {s.m_structs[0].m_name, s.m_structs[0].m_representation};
		case Structure::Function:
			REQUIRE(s.m_functions.size() == 1);
			return {s.m_functions[0].second.m_name,
			        s.m_functions[0].second.m_representation};
		case Structure::Namespace: break;
	}
	return {"default name", "default structure"};
}

std::pair<std::string, std::string>
getNameAndRepresentation(IRProxy::Structure const& structure,
                         IR::Namespace const& ns) {
	using IRProxy::Structure;
	switch (structure) {
		case Structure::Namespace:
			REQUIRE(ns.m_namespaces.size() == 1);
			return {ns.m_namespaces[0].m_name,
			        ns.m_namespaces[0].m_representation};
			break;
		case Structure::Struct:
			REQUIRE(ns.m_structs.size() == 1);
			return {ns.m_structs[0].m_name, ns.m_structs[0].m_representation};
			break;
		case Structure::Function:
			REQUIRE(ns.m_functions.size() == 1);
			return {ns.m_functions[0].m_name,
			        ns.m_functions[0].m_representation};
			break;
	}
	return {"default name", "default structure"};
}
}
