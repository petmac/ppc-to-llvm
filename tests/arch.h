#pragma once

#include <array>

typedef uint32_t Address;
typedef int64_t R;

static const size_t R_COUNT = 32;
static const char *const address_type = "i32";
static const char *const r_type = "i64";

struct State {
	std::array<R, R_COUNT> r;
	Address pc = 0;
};
