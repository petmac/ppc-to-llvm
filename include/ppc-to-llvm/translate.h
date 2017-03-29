#pragma once

#include <ostream>

struct Disassembly;

bool translate(std::ostream &out, const Disassembly &disassembly);
