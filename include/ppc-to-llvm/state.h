#pragma once

#include "address.h"

#include <array>

typedef int64_t R;

constexpr size_t R_COUNT = 32;

struct State {
	std::array<R, R_COUNT> r;
	Address pc = 0;
};
