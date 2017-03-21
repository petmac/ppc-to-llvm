#include "ppc-to-llvm/translation.h"

#include "disassembly.h"

std::string translate(const void *binary, size_t binary_size, uint64_t address) {
	const Disassembly disassembly = disassemble(binary, binary_size, address);
	
	return "TODO WRITE EXPECTED LLVM IR\n";
}
