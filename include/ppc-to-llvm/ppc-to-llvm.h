#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <ostream>

struct cs_insn;

typedef std::shared_ptr<size_t> Capstone;
typedef std::shared_ptr<cs_insn> Instructions;

enum Bits {
	b32,
	b64
};

struct Arch {
	Bits address_bits = b32;
	Bits r_bits = b64;
};

struct Disassembly {
	Capstone cs;
	Instructions insn;
	size_t insn_count = 0;
};

Disassembly disassemble(const void *binary, size_t binary_size, uint64_t address, const Arch &arch);
bool translate(std::ostream &out, const Disassembly &disassembly, const Arch &arch);
