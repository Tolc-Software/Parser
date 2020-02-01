#include "Helpers/Utils/split.h"
#include "catch2/catch.hpp"

namespace {
using SplitContainer = decltype(Helpers::Utils::split("", ""));
void checkSplitHasValues(SplitContainer a,
                         std::vector<SplitContainer::value_type> b) {
	REQUIRE(a.size() == b.size());
	for (auto& referenceValue : b) {
		auto splitValue = a.front();
		CHECK(splitValue == referenceValue);
		a.pop_front();
	}
}
}    // namespace

TEST_CASE("Split works for splitting namespaces", "[split]") {
	auto deque = Helpers::Utils::split("Helpers::Utils::split", "::");
	checkSplitHasValues(deque, {"Helpers", "Utils", "split"});
}
