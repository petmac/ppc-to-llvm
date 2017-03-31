#include "arch.h"
#include "files.h"

#include "ppc-to-llvm/disassemble.h"
#include "ppc-to-llvm/disassembly.h"
#include "ppc-to-llvm/translate.h"

#include <iostream>
#include <fstream>

static const char *const indent = "\t";

static void output_state_ptrs(std::ostream &out) {
	output_register_ptrs(out, "foo", r_type, 0, R_COUNT);
	out << indent << "%pc = getelementptr inbounds %State, %State* %state, i32 0, i32 1" << std::endl;
}

int main(int argc, const char *argv[]) {
	if (argc < 3) {
		std::cerr << "Usage: " << argv[0] << " <input.bin> <output.ll>" << std::endl;
		return 1;
	}
	
	const std::vector<char> binary = load_binary_file(argv[1]);
	
	const Address address = 0x80000000; // TODO Get this from somewhere.
	const Disassembly disassembly = disassemble(binary.data(), binary.size(), address);
	
	std::ofstream out(argv[2]);
	if (out.fail()) {
		std::cerr << "Couldn't open " << argv[2] << std::endl;
		return 2;
	}
	
	out << "%State = type { ";
	out << "[" << R_COUNT << " x " << r_type << "], "; // r
	out << address_type; // pc
	out << " }" << std::endl;
	out << std::endl;
	
	Arch arch;
	arch.output_state_ptrs = output_state_ptrs;
	
	if (!translate(out, disassembly, arch)) {
		std::cerr << "Couldn't translate " << argv[2] << std::endl;
		return 3;
	}
	
	return 0;
}
