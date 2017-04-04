#pragma once

#include <array>
#include <sstream>

template <typename R, typename Address>
struct State {
	std::array<R, 32> r;
	Address pc = 0;
};

template <typename T, size_t N>
void output_nonzero_elements(std::ostream &out, const std::array<T, N> &array, const char *prefix) {
	for (size_t i = 0; i < N; ++i) {
		const T &element = array[i];
		if (element != 0) {
			out << prefix << i << " = " << element << std::endl;
		}
	}
}

template <typename R, typename Address>
std::string to_string(const State<R, Address> &state) {
	std::ostringstream out;
	output_nonzero_elements(out, state.r, "r");
	return out.str();
}
