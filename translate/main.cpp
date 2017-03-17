#include "translation.h"

#include <fstream>
#include <iostream>
#include <vector>

static void usage(const char *path) {
	std::cerr << path << " <input.bin> <output.ll>" << std::endl;
}

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
		usage(argv[0]);
		return 1;
	}
	
	const std::vector<char> binary = load_binary_file(argv[1]);
	const uint64_t address = 0x80000000;
	const std::string translation = translate(binary.data(), binary.size(), address);
	
	std::ofstream out(argv[2]);
	if (out.fail()) {
		std::cerr << "Failed to open output file." << std::endl;
		return 1;
	}
	
	out << "declare void @llvm.debugtrap() nounwind" << std::endl;
	out << std::endl;
	out << "define internal void @run(i64 %address) {" << std::endl;
	out << translation;
	out << "\tcall void @llvm.debugtrap()" << std::endl;
	out << "\tret void" << std::endl;
	out << "}" << std::endl;
	out << std::endl;
	out << "define i32 @main() {" << std::endl;
	out << "\tcall void @run(i64 " << address << ")" << std::endl;
	out << "\tret i32 0" << std::endl;
	out << "}" << std::endl;
	
	return 0;
}
