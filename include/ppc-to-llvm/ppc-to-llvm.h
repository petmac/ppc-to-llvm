#pragma once

#include <memory>
#include <ostream>
#include <stddef.h>
#include <stdint.h>

struct cs_insn;

typedef std::shared_ptr<size_t> Capstone;
typedef std::shared_ptr<cs_insn> Instructions;

struct Arch {
	std::string address_type = "i32";
	std::string r_type = "i64";
};

struct Disassembly {
	Capstone cs;
	Instructions insn;
	size_t insn_count = 0;
};

Disassembly disassemble(const void *binary, size_t binary_size, uint64_t address);
bool translate(std::ostream &out, const Disassembly &disassembly, const Arch &arch);
