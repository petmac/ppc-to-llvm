#pragma once

#include <cassert>
#include <ostream>

#include <capstone/ppc.h>

#define OP_FN(id) void op_##id(std::ostream &out, const cs_ppc_op *operands, size_t op_count, const char *arch_type)

#define OP(id) OP_FN(id);
#include "ops.h"
#undef OP
