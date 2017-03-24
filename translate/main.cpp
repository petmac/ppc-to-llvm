#include "ppc-to-llvm/disassemble.h"
#include "ppc-to-llvm/disassembly.h"
#include "ppc-to-llvm/translate.h"
#include "ppc-to-llvm/translation.h"

#include "llvm/IR/Module.h"
#include "llvm/Support/raw_os_ostream.h"

#include <iostream>
#include <fstream>
#include <vector>

static std::vector<char> load_binary_file(const char *path) {
	std::ifstream file(path, std::ios::binary);
	if (file.fail()) {
		throw std::runtime_error("File open failed.");
	}
	
	file.seekg(0, std::ios::end);
	const size_t buffer_size = file.tellg();
	file.seekg(0);
	
	std::vector<char> buffer(buffer_size);
	file.read(buffer.data(), buffer.size());
	
	return buffer;
}

int main(int argc, const char *argv[]) {
	if (argc < 3) {
		std::cerr << "Usage: " << argv[0] << " <input.bin> <output.ll>" << std::endl;
		return 1;
	}
	
	const std::vector<char> binary = load_binary_file(argv[1]);
	
	const uint64_t address = 0x80000000; // TODO Get this from somewhere.
	const Disassembly disassembly = disassemble(binary.data(), binary.size(), address);
	const Translation translation = translate(disassembly);
	
	std::ofstream file(argv[2]);
	if (file.fail()) {
		std::cerr << "Couldn't open " << argv[2] << std::endl;
		return 2;
	}
	
	llvm::raw_os_ostream out(file);
	out << *translation.module;
	
	return 0;
}
