#include "data.h"
#include "state.h"

#include "ppc-to-llvm/files.h"

#include "gtest/gtest.h"

#include <dlfcn.h>

typedef std::unique_ptr<void, std::function<void(void *)>> DLL;

static const char data_dir[] = "data/";

static std::string build_path(const char *data_name, const char *suffix) {
	std::ostringstream path;
	path << data_dir << data_name << suffix;
	return path.str();
}

static std::string build_path(const char *data_name, size_t arch_bits, const char *suffix) {
	std::ostringstream path;
	path << data_dir << data_name << "-ppc" << arch_bits << suffix;
	return path.str();
}

static std::string build_path(const char *data_name, size_t arch_bits, size_t address_bits, const char *suffix) {
	std::ostringstream path;
	path << data_dir << data_name << "-ppc" << arch_bits << "-addr" << address_bits << suffix;
	return path.str();
}

static DLL load_dll(const char *data_name, size_t arch_bits, size_t address_bits) {
	const std::string path = build_path(data_name, arch_bits, address_bits, ".dylib");
	return DLL(dlopen(path.c_str(), RTLD_NOW), dlclose);
}

static std::string load_expected_state(const char *data_name, size_t arch_bits, size_t address_bits) {
	const char *const suffix = "-state.txt";
	const std::string arch_address_path = build_path(data_name, arch_bits, address_bits, suffix);
	try {
		return load_text_file(arch_address_path.c_str());
	} catch (const std::runtime_error &) {
		const std::string arch_path = build_path(data_name, arch_bits, suffix);
		try {
			return load_text_file(arch_path.c_str());
		} catch (const std::runtime_error &) {
			const std::string path = build_path(data_name, suffix);
			return load_text_file(path.c_str());
		}
	}
}

static std::string test_name(const testing::TestParamInfo<const char *> &param) {
	return param.param;
}

template <typename R, typename Address>
static void test(const char *data_name, size_t arch_bits, size_t address_bits) {
	typedef State<R, Address> State;
	typedef void RunFn(State *state);
	
	const DLL dll = load_dll(data_name, arch_bits, address_bits);
	ASSERT_TRUE(dll);
	
	RunFn *const run = reinterpret_cast<RunFn *>(dlsym(dll.get(), "run"));
	ASSERT_NE(run, nullptr);
	
	State state = {};
	state.pc = 0x80000000; // TODO Get this from somewhere.
	(*run)(&state);
	
	const std::string actual_state = to_string(state);
	const std::string expected_state = load_expected_state(data_name, arch_bits, address_bits);
	ASSERT_EQ(actual_state, expected_state);
}

class Tests : public testing::TestWithParam<const char *> {
};

TEST_P(Tests, ppc32_addr32) {
	test<int32_t, uint32_t>(GetParam(), 32, 32);
}

TEST_P(Tests, ppc32_addr64) {
	test<int32_t, uint64_t>(GetParam(), 32, 64);
}

TEST_P(Tests, ppc64_addr32) {
	test<int64_t, uint32_t>(GetParam(), 64, 32);
}

TEST_P(Tests, ppc64_addr64) {
	test<int64_t, uint64_t>(GetParam(), 64, 64);
}

INSTANTIATE_TEST_CASE_P(, Tests, testing::ValuesIn(data_names), test_name);
