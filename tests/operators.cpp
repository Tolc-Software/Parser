#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"
#include <IR/ir.hpp>
#include <algorithm>
#include <catch2/catch.hpp>

void requireContainsOperators(IR::Namespace& ns,
                              std::vector<IR::Operator> const& operators) {
	auto const& opStruct = TestUtil::findStruct(ns, "Operator");
	auto const& publicOperators = opStruct.m_public.m_operators;
	REQUIRE(publicOperators.size() == operators.size());
	for (auto const& op : operators) {
		auto it = std::find_if(publicOperators.begin(),
		                       publicOperators.end(),
		                       [op](auto const& opAndFunction) {
			                       return opAndFunction.first == op;
		                       });
		REQUIRE(it != publicOperators.end());
	}
}

TEST_CASE("Operators gets parsed", "[operators]") {
	auto globalNS = TestUtil::parseString(R"(
struct Operator {
  // +-*/&
  Operator operator+(int);
  Operator operator-(int);
  Operator operator*(int);
  Operator operator/(int);
  Operator operator%(int);

  // Assignment
  Operator& operator=(Operator&& other) noexcept;

  // Comparisons
  bool operator==(const Operator& rhs);
  bool operator!=(const Operator& rhs);
  bool operator< (const Operator& rhs);
  bool operator> (const Operator& rhs);
  bool operator<=(const Operator& rhs);
  bool operator>=(const Operator& rhs);

  // Subscript
  Operator& operator[](unsigned idx);

  // Call
  double operator()(double x);
};
)");
	using IR::Operator;
	requireContainsOperators(globalNS,
	                         {Operator::Addition,
	                          Operator::Subtraction,
	                          Operator::Multiplication,
	                          Operator::Division,
	                          Operator::Modulus,
	                          Operator::Assignment,
	                          Operator::Equal,
	                          Operator::NotEqual,
	                          Operator::LessThan,
	                          Operator::LessThanOrEqualTo,
	                          Operator::GreaterThan,
	                          Operator::GreaterThanOrEqualTo,
	                          Operator::Subscript,
	                          Operator::Call});
}
