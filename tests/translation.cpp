#include "files.h"

#include "ppc-to-llvm/disassemble.h"
#include "ppc-to-llvm/disassembly.h"
#include "ppc-to-llvm/translate.h"

#include "gtest/gtest.h"

#include <fstream>

class Translation : public testing::TestWithParam<const char *> {
public:
	
	virtual void SetUp() {
		const char *const data_name = GetParam();
		const char *const data_root = "data/translation/";
		std::string data_base_path(data_root);
		data_base_path.append(data_name);
		
		std::string binary_path(data_base_path);
		binary_path.append(".bin");
		
		std::string expected_ll_path(data_base_path);
		expected_ll_path.append(".ll");
		
		binary = load_binary_file(binary_path.c_str());
		expected_ll = load_text_file(expected_ll_path.c_str());
	}
	
protected:
	
	std::vector<char> binary;
	std::string expected_ll;
};

TEST_P(Translation, matches_expected) {
	const uint64_t address = 0x80000000; // TODO Get this from somewhere.
	const Disassembly disassembly = disassemble(binary.data(), binary.size(), address);
	const std::string translated = translate(disassembly);
	
	EXPECT_EQ(translated, expected_ll);
}

INSTANTIATE_TEST_CASE_P(, Translation, testing::Values("blr"));
