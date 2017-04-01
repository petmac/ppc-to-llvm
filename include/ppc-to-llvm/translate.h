#pragma once

#include <ostream>

struct Disassembly;

struct Arch {
	std::string address_type = "i32";
	std::string r_type = "i64";
};

bool translate(std::ostream &out, const Disassembly &disassembly, const Arch &arch);
