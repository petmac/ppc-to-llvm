#include "ppc-to-llvm/files.h"

#include <fstream>
#include <sstream>

std::vector<char> load_binary_file(const char *path) {
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

std::string load_text_file(const char *path) {
	std::ifstream file(path);
	if (file.fail()) {
		throw std::runtime_error("File open failed.");
	}
	
	std::ostringstream buffer;
	buffer << file.rdbuf();
	
	return buffer.str();
}
