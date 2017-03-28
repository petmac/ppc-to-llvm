#include "files.h"

#include "gtest/gtest.h"

#include <dlfcn.h>
#include <functional>
#include <memory>

typedef void RunFn();

class CorrectnessTests : public testing::TestWithParam<const char *> {
public:
	
	virtual void SetUp() {
		const char *const data_name = GetParam();
		const char *const data_root = "data/correctness/";
		std::string data_base_path(data_root);
		data_base_path.append(data_name);
		
		std::string binary_path(data_base_path);
		binary_path.append(".bin");
		
		std::string dll_path(data_base_path);
		dll_path.append(".dylib");
		
		binary = load_binary_file(binary_path.c_str());
		dll = std::unique_ptr<void, std::function<void(void *)>>(dlopen(dll_path.c_str(), RTLD_NOW), dlclose);
		run = reinterpret_cast<RunFn *>(dlsym(dll.get(), "main"));
	}
	
protected:
	
	std::vector<char> binary;
	std::unique_ptr<void, std::function<void(void *)>> dll;
	RunFn *run = nullptr;
};

TEST_P(CorrectnessTests, matches_expected) {
	(*run)();
}

INSTANTIATE_TEST_CASE_P(, CorrectnessTests, testing::Values("empty"));
