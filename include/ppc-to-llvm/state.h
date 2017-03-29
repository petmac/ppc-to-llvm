#pragma once

#include <array>

constexpr size_t R_COUNT = 32;

struct State {
	std::array<uint64_t, R_COUNT> r;
};
