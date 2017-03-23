#include "files.h"

#include "ppc-to-llvm/disassemble.h"
#include "ppc-to-llvm/disassembly.h"
#include "ppc-to-llvm/translate.h"
#include "ppc-to-llvm/translation.h"

#include "gtest/gtest.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_os_ostream.h"

class TranslationTests : public testing::TestWithParam<const char *> {
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

TEST_P(TranslationTests, matches_expected) {
	const uint64_t address = 0x80000000; // TODO Get this from somewhere.
	const Disassembly disassembly = disassemble(binary.data(), binary.size(), address);
	const Translation translation = translate(disassembly);
	
	std::ostringstream actual_ll;
	llvm::raw_os_ostream out(actual_ll);
	out << *translation.module;
	
	EXPECT_EQ(actual_ll.str(), expected_ll);
}

INSTANTIATE_TEST_CASE_P(, TranslationTests, testing::Values("blr"));
