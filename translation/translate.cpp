#include "ppc-to-llvm/translate.h"

#include "ppc-to-llvm/disassembly.h"

bool translate(std::ostream &out, const Disassembly &disassembly) {
	const std::string r_type = "i64";
	const std::string indent = "\t";
	
	out << "%State = type { [32 x " << r_type << "] }" << std::endl;
	out << std::endl;
	out << "define dllexport void @run(%State* %state) {" << std::endl;
	out << indent << "ret void" << std::endl;
	out << "}" << std::endl;
	
	return true;
}
