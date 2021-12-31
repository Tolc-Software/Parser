#pragma once

#include "IRProxy/IRData.hpp"
#include <IR/ir.hpp>

namespace Helpers {

/**
* Find the data prefixed with accessModifier (i.e. public/private/protected)
* This contains functions etc.
* Returns nullptr if not found (should not happen)
*/
IR::StructData* getStructDataBasedOnAccess(IR::Struct& s,
                                           IRProxy::AccessModifier access);
}