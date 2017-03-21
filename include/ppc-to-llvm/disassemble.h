#pragma once

#include <stddef.h>
#include <stdint.h>

struct Disassembly;

Disassembly disassemble(const void *binary, size_t binary_size, uint64_t address);
