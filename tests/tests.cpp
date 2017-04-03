#include "tests.h"

#include "data.h"

void Tests::SetUp() {
	const char *const data_name = std::get<0>(GetParam());
	const size_t arch_bits = std::get<1>(GetParam());
	const size_t address_bits = std::get<2>(GetParam());
	
	std::ostringstream dll_path;
	dll_path << "data/" << data_name << "-" << arch_bits << "-" << address_bits << ".dylib";
	
	dll = std::unique_ptr<void, std::function<void(void *)>>(dlopen(dll_path.str().c_str(), RTLD_NOW), dlclose);
	ASSERT_TRUE(dll);
}

TEST_P(Tests, matches_expected) {
	const size_t arch_bits = std::get<1>(GetParam());
	const size_t address_bits = std::get<2>(GetParam());
	
	if (arch_bits == 32) {
		if (address_bits == 32) {
			matches_expected<int32_t, uint32_t>();
		} else if (address_bits == 64) {
			matches_expected<int32_t, uint64_t>();
		} else {
			FAIL();
		}
	} else if (arch_bits == 64) {
		if (address_bits == 32) {
			matches_expected<int64_t, uint32_t>();
		} else if (address_bits == 64) {
			matches_expected<int64_t, uint64_t>();
		} else {
			FAIL();
		}
	} else {
		FAIL();
	}
}

INSTANTIATE_TEST_CASE_P(, Tests, testing::Combine(testing::ValuesIn(data_names), testing::Values(32, 64), testing::Values(32, 64)));
