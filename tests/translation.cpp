#include "ppc-to-llvm/disassemble.h"
#include "ppc-to-llvm/disassembly.h"
#include "ppc-to-llvm/translate.h"
#include "ppc-to-llvm/translation.h"

#include "gtest/gtest.h"
#include "llvm/IR/LLVMContext.h"

#include <fstream>

static std::vector<char> load_binary_file(const char *path) {
	std::ifstream file(path, std::ios::binary);
	if (file.fail()) {
		throw std::runtime_error("File open failed.");
	}
	
	file.seekg(0, std::ios::end);
	const size_t buffer_size = file.tellg();
	file.seekg(0);
	
	std::vector<char> buffer(buffer_size);
	file.read(buffer.data(), buffer.size());
	
	return buffer;
}

static std::string load_text_file(const char *path) {
	std::ifstream file(path);
	if (file.fail()) {
		throw std::runtime_error("File open failed.");
	}
	
	std::stringstream buffer;
	buffer << file.rdbuf();
	
	return buffer.str();
}

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
	
	EXPECT_EQ("TODO", expected_ll);
}

INSTANTIATE_TEST_CASE_P(, TranslationTests, testing::Values("blr"));
