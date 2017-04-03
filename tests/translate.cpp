#include "files.h"

#include "ppc-to-llvm/ppc-to-llvm.h"

#include <iostream>
#include <map>
#include <fstream>

static const std::map<std::string, Bits> str_to_bits = {
	{ "32", b32 },
	{ "64", b64 }
};

int main(int argc, const char *argv[]) {
	if (argc < 5) {
		std::cerr << "Usage: " << argv[0] << " <arch bits> <address bits> <input.bin> <output.ll>" << std::endl;
		return 1;
	}
	
	const Bits arch_bits = str_to_bits.at(argv[1]);
	const Bits address_bits = str_to_bits.at(argv[2]);
	const char *const input_path = argv[3];
	const char *const output_path = argv[4];
	
	Arch arch;
	arch.arch_bits = arch_bits;
	arch.address_bits = address_bits;
	
	const std::vector<char> binary = load_binary_file(input_path);
	const uint64_t address = 0x80000000; // TODO Get this from somewhere.
	const Disassembly disassembly = disassemble(binary.data(), binary.size(), address, arch);
	
	std::ofstream out(output_path);
	if (out.fail()) {
		std::cerr << "Couldn't open " << output_path << std::endl;
		return 2;
	}
	
	if (!translate(out, disassembly, arch)) {
		std::cerr << "Couldn't translate " << input_path << std::endl;
		return 3;
	}
	
	return 0;
}
