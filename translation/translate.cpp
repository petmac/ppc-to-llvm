#include "ppc-to-llvm/translate.h"

#include "ppc-to-llvm/disassembly.h"

std::string translate(const Disassembly &disassembly) {
	return "define void @run() {\n\tret void\n}\n";
}
