#include "files.h"
#include "state.h"

#include "gtest/gtest.h"

#include <dlfcn.h>

#include <functional>
#include <memory>
#include <sstream>

class CorrectnessTests : public testing::TestWithParam<std::tuple<const char *, size_t, size_t>> {
public:
	
	virtual void SetUp() {
		const char *const data_name = std::get<0>(GetParam());
		const size_t arch_bits = std::get<1>(GetParam());
		const size_t address_bits = std::get<2>(GetParam());
		
		std::ostringstream path_prefix;
		path_prefix << "data/correctness/" << data_name << "-" << arch_bits;
		
		std::string binary_path = path_prefix.str();
		binary_path.append(".bin");
		
		path_prefix << "-" << address_bits;
		
		std::string dll_path = path_prefix.str();
		dll_path.append(".dylib");
		
		binary = load_binary_file(binary_path.c_str());
		dll = std::unique_ptr<void, std::function<void(void *)>>(dlopen(dll_path.c_str(), RTLD_NOW), dlclose);
		ASSERT_TRUE(dll);
	}
	
protected:
	
	std::vector<char> binary;
	std::unique_ptr<void, std::function<void(void *)>> dll;
	
	template <typename R, typename Address>
	void matches_expected() {
		typedef State<R, Address> State;
		typedef void RunFn(State *state);
		
		RunFn *const run = reinterpret_cast<RunFn *>(dlsym(dll.get(), "run"));
		ASSERT_NE(run, nullptr);
		
		State state = {};
		state.pc = 0x80000000; // TODO Get this from somewhere.
		(*run)(&state);
		ASSERT_EQ(state.pc, 0x80000000);
	}
};

TEST_P(CorrectnessTests, matches_expected) {
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

INSTANTIATE_TEST_CASE_P(, CorrectnessTests, testing::Combine(testing::Values("empty"), testing::Values(32, 64), testing::Values(32, 64)));
