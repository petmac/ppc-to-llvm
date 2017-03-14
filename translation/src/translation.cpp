#include "translation.h"

#include "disassembly.h"

std::string translate(const void *binary, size_t binary_size) {
	const Disassembly disassembly = disassemble(binary, binary_size);
	
	return "TODO TRANSLATE";
}
