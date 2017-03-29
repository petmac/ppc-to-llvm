#include "ppc-to-llvm/state.h"
#include "ppc-to-llvm/translate.h"

#include "ppc-to-llvm/disassembly.h"

bool translate(std::ostream &out, const Disassembly &disassembly) {
	const std::string r_type = "i64";
	const std::string address_type = "i32";
	const std::string indent = "\t";
	
	out << "declare void @llvm.debugtrap() nounwind" << std::endl;
	out << std::endl;
	out << "%State = type { ";
	out << "[" << R_COUNT << " x " << r_type << "], "; // r
	out << address_type; // pc
	out << " }" << std::endl;
	out << std::endl;
	out << "define dllexport void @run(%State* %state) {" << std::endl;
	out << indent << "%r = getelementptr inbounds %State, %State* %state, i32 0, i32 0, i32 0" << std::endl;
	for (size_t r = 0; r < R_COUNT; ++r) {
		out << indent << "%r" << r << " = getelementptr inbounds " << r_type << ", " << r_type << "* %r, i32 " << r << std::endl;
	}
	out << indent << "%pc = getelementptr inbounds %State, %State* %state, i32 0, i32 1" << std::endl;
	out << indent << "store " << address_type << " 123, " << address_type << "* %pc" << std::endl;
	out << indent << "ret void" << std::endl;
	out << "}" << std::endl;
	
	return true;
}
