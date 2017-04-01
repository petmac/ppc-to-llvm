#pragma once

#include <array>

typedef uint32_t Address;
typedef int64_t R;

struct State {
	std::array<R, 32> r;
	Address pc = 0;
};
