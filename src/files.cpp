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

bool load_text_file(std::string &out, const char *path) {
	std::ifstream file(path);
	if (file.fail()) {
		return false;
	}
	
	std::ostringstream buffer;
	buffer << file.rdbuf();
	
	out = buffer.str();
	return true;
}
