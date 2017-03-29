#pragma once

#include <array>

typedef int64_t R;

constexpr size_t R_COUNT = 32;

struct State {
	std::array<R, R_COUNT> r;
};
