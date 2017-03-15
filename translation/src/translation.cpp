#include "translation.h"

#include "disassembly.h"

std::string translate(const void *binary, size_t binary_size, uint64_t address) {
	const Disassembly disassembly = disassemble(binary, binary_size, address);
	
	return
	"define i32 @main() {\n"
	"\tret i32 0\n"
	"}\n";
}
