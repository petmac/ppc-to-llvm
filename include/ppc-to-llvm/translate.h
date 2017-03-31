#pragma once

#include <functional>
#include <ostream>

struct Disassembly;

typedef std::function<void(std::ostream &)> OutputStatePtrs;

struct Arch {
	std::string address_type = "i32";
	OutputStatePtrs output_state_ptrs;
};

bool translate(std::ostream &out, const Disassembly &disassembly, const Arch &arch);
void output_register_ptrs(std::ostream &out, const char *prefix, const char *type, size_t member, size_t count);
