#pragma once

#include <memory>

struct cs_insn;

typedef std::shared_ptr<size_t> Capstone;
typedef std::shared_ptr<cs_insn> Instructions;

struct Disassembly {
	Capstone cs;
	Instructions insn;
	size_t insn_count = 0;
};

Disassembly disassemble(const void *binary, size_t binary_size, uint64_t address);
