#pragma once

#include <IR/ir.hpp>
#include <optional>
#include <string_view>

namespace Helpers::Type {

// Tries to match a base type to the string and return it
// Base types: Int, Void, ...
std::optional<IR::Type::Value> getValueType(std::string_view type);

// Tries to match the input to a user defined struct type
std::optional<IR::Type::UserDefined> getUserDefinedType(std::string_view type);

// Tries to match a container type to the string and return it
// Base types: Vector, Map, ...
std::optional<IR::ContainerType> getContainerType(std::string_view type);

}    // namespace Helpers::Type
