#include "ppc-to-llvm/state.h"
#include "ppc-to-llvm/translate.h"

#include "ppc-to-llvm/disassembly.h"

#include <capstone/capstone.h>

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

static void output_switch(std::ostream &out, const Disassembly &disassembly) {
	const std::string pc = "%1";
	out << indent << pc << " = load " << address_type << ", " << address_type << "* %pc" << std::endl;
	out << indent << "switch " << address_type << " " << pc << ", label %badpc [";
	if (disassembly.insn_count > 0) {
		const cs_insn &first_insn = *disassembly.insn;
		out << address_type << " " << first_insn.address << ", label %insn0";
		for (size_t insn_index = 1; insn_index < disassembly.insn_count; ++insn_index) {
			const cs_insn &insn = disassembly.insn.get()[insn_index];
			out << " " << address_type << " " << insn.address << ", label %insn" << insn_index;
		}
	}
	out << "]" << std::endl;
}

static void output_instructions(std::ostream &out, const Disassembly &disassembly) {
	for (size_t insn_index = 0; insn_index < disassembly.insn_count; ++insn_index) {
		out << "insn" << insn_index << ":" << std::endl;
		out << indent << "store " << address_type << " 123, " << address_type << "* %pc" << std::endl;
		out << indent << "br label %loop" << std::endl;
	}
}

static void output_run(std::ostream &out, const Disassembly &disassembly) {
	out << "define dllexport void @run(%State* %state) {" << std::endl;
	output_registers(out);
	out << indent << "br label %loop" << std::endl;
	
	out << "loop:" << std::endl;
	output_switch(out, disassembly);
	
	out << "badpc:" << std::endl;
	out << indent << "ret void" << std::endl;
	
	output_instructions(out, disassembly);
	out << "}" << std::endl;
}

bool translate(std::ostream &out, const Disassembly &disassembly) {
	output_declarations(out);
	out << std::endl;
	output_run(out, disassembly);
	
	return true;
}
