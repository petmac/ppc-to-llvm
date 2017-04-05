#pragma once

#include <ostream>

#define OP_FN(id) void op_##id(std::ostream &out)

#define OP(id) OP_FN(id);
#include "ops.h"
#undef OP
