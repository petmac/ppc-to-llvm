#pragma once

#include <array>

template <typename R, typename Address>
struct State {
	std::array<R, 32> r;
	Address pc = 0;
};
