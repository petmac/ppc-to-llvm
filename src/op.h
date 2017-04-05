#pragma once

#include <ostream>

#include <capstone/ppc.h>

#define OP_FN(id) void op_##id(std::ostream &out, const cs_ppc_op *operands, size_t op_count)

#define OP(id) OP_FN(id);
#include "ops.h"
#undef OP
