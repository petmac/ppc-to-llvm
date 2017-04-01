#include "files.h"

#include "ppc-to-llvm/ppc-to-llvm.h"

#include <iostream>
#include <fstream>

int main(int argc, const char *argv[]) {
	if (argc < 3) {
		std::cerr << "Usage: " << argv[0] << " <input.bin> <output.ll>" << std::endl;
		return 1;
	}
	
	const Arch arch {};
	const std::vector<char> binary = load_binary_file(argv[1]);
	const uint64_t address = 0x80000000; // TODO Get this from somewhere.
	const Disassembly disassembly = disassemble(binary.data(), binary.size(), address, arch);
	
	std::ofstream out(argv[2]);
	if (out.fail()) {
		std::cerr << "Couldn't open " << argv[2] << std::endl;
		return 2;
	}
	
	if (!translate(out, disassembly, arch)) {
		std::cerr << "Couldn't translate " << argv[2] << std::endl;
		return 3;
	}
	
	return 0;
}
