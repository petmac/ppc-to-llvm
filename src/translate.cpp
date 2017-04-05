#include "ppc-to-llvm/ppc-to-llvm.h"

#include <capstone/capstone.h>

#include <map>

struct TranslateArch {
	std::string arch_type;
	std::string address_type;
};

static const size_t R_COUNT = 32;
static const std::string indent = "\t";

static void output_declarations(std::ostream &out, const TranslateArch &arch) {
	out << "declare void @llvm.debugtrap() nounwind" << std::endl;
	out << std::endl;
	out << "%State = type { ";
	out << "[" << R_COUNT << " x " << arch.arch_type << "], "; // r
	out << arch.address_type; // pc
	out << " }" << std::endl;
}

static void output_switch(std::ostream &out, const Disassembly &disassembly, const char *address_type) {
	const char pc[] = "%pc_";
	const char pc_relative_to_bin[] = "%pc_offset";
	const char insn_index[] = "%insn_index";
	
	out << indent << pc << " = load " << address_type << ", " << address_type << "* %pc" << std::endl;
	out << indent << pc_relative_to_bin << " = sub " << address_type << " " << pc << ", " << disassembly.insn->address << std::endl;
	out << indent << insn_index << " = udiv " << address_type << " " << pc_relative_to_bin << ", 4" << std::endl;
	out << indent << "switch " << address_type << " " << insn_index << ", label %badpc [";
	
	for (size_t insn_index = 0; insn_index < disassembly.insn_count; ++insn_index) {
		out << address_type << " " << insn_index << ", label %insn" << insn_index << " ";
	}
	
	out << address_type << " " << disassembly.insn_count << ", label %exit";
	
	out << "]" << std::endl;
}

static bool translate_instruction(std::ostream &out, const cs_insn &insn, const char *address_type) {
	const uint64_t next_insn_address = insn.address + insn.size;
	out << indent << "store " << address_type << " " << next_insn_address << ", " << address_type << "* %pc" << std::endl;
	out << indent << "br label %loop" << std::endl;
	
	return true;
}

static bool translate_instructions(std::ostream &out, const Disassembly &disassembly, const char *address_type) {
	for (size_t insn_index = 0; insn_index < disassembly.insn_count; ++insn_index) {
		const cs_insn &insn = disassembly.insn.get()[insn_index];
		out << "insn" << insn_index << ": ; " << insn.mnemonic;
		if (insn.op_str[0] != '\0') {
			out << " " << insn.op_str;
		}
		out << std::endl;
		if (!translate_instruction(out, insn, address_type)) {
			return false;
		}
	}
	
	return true;
}

static void output_register_ptrs(std::ostream &out, const char *prefix, const char *type, size_t member, size_t count) {
	out << indent << "%" << prefix << " = getelementptr inbounds %State, %State* %state, i32 0, i32 " << member << ", i32 0" << std::endl;
	for (size_t index = 0; index < count; ++index) {
		out << indent << "%" << prefix << index << " = getelementptr inbounds " << type << ", " << type << "* %" << prefix << ", i32 " << index << std::endl;
	}
}

static bool output_run(std::ostream &out, const Disassembly &disassembly, const TranslateArch &arch) {
	out << "define dllexport void @run(%State* %state) {" << std::endl;
	if (disassembly.insn_count > 0) {
		output_register_ptrs(out, "r", arch.arch_type.c_str(), 0, R_COUNT);
		out << indent << "%pc = getelementptr inbounds %State, %State* %state, i32 0, i32 1" << std::endl;
		out << indent << "br label %loop" << std::endl;
		
		out << "loop:" << std::endl;
		output_switch(out, disassembly, arch.address_type.c_str());
		
		if (!translate_instructions(out, disassembly, arch.address_type.c_str())) {
			return false;
		}
		
		out << "exit:" << std::endl;
		out << indent << "ret void" << std::endl;
		
		out << "badpc:" << std::endl;
		out << indent << "call void @llvm.debugtrap()" << std::endl;
		out << indent << "ret void" << std::endl;
	} else {
		out << indent << "ret void" << std::endl;
	}
	
	out << "}" << std::endl;
	
	return true;
}

static const std::map<Bits, const char *> bits_to_type = {
	{ b32, "i32" },
	{ b64, "i64" },
};

bool translate(std::ostream &out, const Disassembly &disassembly, const Arch &arch) {
	TranslateArch translate_arch;
	translate_arch.arch_type = bits_to_type.at(arch.arch_bits);
	translate_arch.address_type = bits_to_type.at(arch.address_bits);
	
	output_declarations(out, translate_arch);
	out << std::endl;
	if (!output_run(out, disassembly, translate_arch)) {
		return false;
	}
	
	return true;
}
