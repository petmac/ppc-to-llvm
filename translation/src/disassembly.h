#pragma once

#include <memory>

typedef std::shared_ptr<size_t> Capstone;

struct Disassembly {
	Capstone cs;
};

Disassembly disassemble(const void *binary, size_t binary_size);
