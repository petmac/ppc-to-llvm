#include "ppc-to-llvm/translate.h"

#include "ppc-to-llvm/disassembly.h"

#include <capstone/capstone.h>

static const std::string indent = "\t";

static void output_declarations(std::ostream &out) {
	out << "declare void @llvm.debugtrap() nounwind" << std::endl;
}

static void output_switch(std::ostream &out, const Disassembly &disassembly, const char *address_type) {
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

static bool translate_instruction(std::ostream &out, const cs_insn &insn, const char *address_type) {
	out << indent << "store " << address_type << " 123, " << address_type << "* %pc" << std::endl;
	
	return true;
}

static bool output_instructions(std::ostream &out, const Disassembly &disassembly, const char *address_type) {
	for (size_t insn_index = 0; insn_index < disassembly.insn_count; ++insn_index) {
		out << "insn" << insn_index << ":" << std::endl;
		if (!translate_instruction(out, disassembly.insn.get()[insn_index], address_type)) {
			return false;
		}
		out << indent << "br label %loop" << std::endl;
	}
	
	return true;
}

static bool output_run(std::ostream &out, const Disassembly &disassembly, const Arch &arch) {
	out << "define dllexport void @run(%State* %state) {" << std::endl;
	if (arch.output_state_ptrs) {
		arch.output_state_ptrs(out);
	}
	out << indent << "br label %loop" << std::endl;
	
	out << "loop:" << std::endl;
	output_switch(out, disassembly, arch.address_type.c_str());
	
	out << "badpc:" << std::endl;
	out << indent << "ret void" << std::endl;
	
	if (!output_instructions(out, disassembly, arch.address_type.c_str())) {
		return false;
	}
	
	out << "}" << std::endl;
	
	return true;
}

bool translate(std::ostream &out, const Disassembly &disassembly, const Arch &arch) {
	output_declarations(out);
	out << std::endl;
	if (!output_run(out, disassembly, arch)) {
		return false;
	}
	
	return true;
}

void output_register_ptrs(std::ostream &out, const char *prefix, const char *type, size_t member, size_t count) {
	out << indent << "%" << prefix << " = getelementptr inbounds %State, %State* %state, i32 0, i32 " << member << ", i32 0" << std::endl;
	for (size_t index = 0; index < count; ++index) {
		out << indent << "%" << prefix << index << " = getelementptr inbounds " << type << ", " << type << "* %" << prefix << ", i32 " << index << std::endl;
	}
}
