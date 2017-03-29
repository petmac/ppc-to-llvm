#pragma once

#include "ppc-to-llvm/address.h"

#include <stddef.h>

struct Disassembly;

Disassembly disassemble(const void *binary, size_t binary_size, Address address);
