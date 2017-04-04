#include "data.h"

#include <sstream>

const std::vector<const char *> data_names = {
	"blr", "empty"
};

std::string build_path(const char *name, const char *suffix) {
	std::ostringstream path;
	path << "data/" << name << suffix;
	return path.str();
}

std::string build_path(const char *name, size_t arch_bits, const char *suffix) {
	std::ostringstream path;
	path << build_path(name, "") << "-ppc" << arch_bits << suffix;
	return path.str();
}

std::string build_path(const char *name, size_t arch_bits, size_t address_bits, const char *suffix) {
	std::ostringstream path;
	path << build_path(name, arch_bits, "") << "-addr" << address_bits << suffix;
	return path.str();
}
