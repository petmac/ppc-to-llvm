#include "ppc-to-llvm/state.h"
#include "ppc-to-llvm/translate.h"

#include "ppc-to-llvm/disassembly.h"

static const std::string address_type = "i32";
static const std::string r_type = "i64";
static const std::string indent = "\t";

static void output_declarations(std::ostream &out) {
	out << "declare void @llvm.debugtrap() nounwind" << std::endl;
	out << std::endl;
	out << "%State = type { ";
	out << "[" << R_COUNT << " x " << r_type << "], "; // r
	out << address_type; // pc
	out << " }" << std::endl;
}

static void output_registers(std::ostream &out) {
	out << indent << "%r = getelementptr inbounds %State, %State* %state, i32 0, i32 0, i32 0" << std::endl;
	for (size_t r = 0; r < R_COUNT; ++r) {
		out << indent << "%r" << r << " = getelementptr inbounds " << r_type << ", " << r_type << "* %r, i32 " << r << std::endl;
	}
	out << indent << "%pc = getelementptr inbounds %State, %State* %state, i32 0, i32 1" << std::endl;
}

static void output_run(std::ostream &out) {
	out << "define dllexport void @run(%State* %state) {" << std::endl;
	output_registers(out);
	out << indent << "store " << address_type << " 123, " << address_type << "* %pc" << std::endl;
	out << indent << "ret void" << std::endl;
	out << "}" << std::endl;
}

bool translate(std::ostream &out, const Disassembly &disassembly) {
	output_declarations(out);
	out << std::endl;
	output_run(out);
	
	return true;
}
