#include "files.h"

#include "ppc-to-llvm/ppc-to-llvm.h"

#include "gtest/gtest.h"

#include <fstream>
#include <sstream>

class Translation : public testing::TestWithParam<const char *> {
public:
	
	virtual void SetUp() {
		const char *const data_name = GetParam();
		const char *const data_root = "data/translation/";
		std::string data_base_path(data_root);
		data_base_path.append(data_name);
		
		std::string binary_path(data_base_path);
		binary_path.append("-64.bin");
		
		binary = load_binary_file(binary_path.c_str());
	}
	
protected:
	
	std::vector<char> binary;
};

TEST_P(Translation, succeeds) {
	const Arch arch {};
	const uint64_t address = 0x80000000; // TODO Get this from somewhere.
	const Disassembly disassembly = disassemble(binary.data(), binary.size(), address, arch);
	
	std::ostringstream translated;
	ASSERT_TRUE(translate(translated, disassembly, arch));
}

INSTANTIATE_TEST_CASE_P(, Translation, testing::Values("blr"));
