#include "ppc-to-llvm/translate.h"

#include "ppc-to-llvm/disassembly.h"

bool translate(std::ostream &out, const Disassembly &disassembly) {
	out << "define dllexport void @run() {" << std::endl;
	out << "\tret void" << std::endl;
	out << "}" << std::endl;
	
	return true;
}
