#include "files.h"

#include "ppc-to-llvm/ppc-to-llvm.h"

#include "gtest/gtest.h"

#include <fstream>
#include <map>
#include <sstream>

static const std::map<Bits, const char *> bits_suffixes = {
	{ b32, "-32" },
	{ b64, "-64" }
};

class Translation : public testing::TestWithParam<std::tuple<const char *, Bits, Bits>> {
public:
	
	virtual void SetUp() {
		const char *const data_name = std::get<0>(GetParam());
		const Bits address_bits = std::get<1>(GetParam());
		const Bits r_bits = std::get<2>(GetParam());
		const char *const data_root = "data/translation/";
		std::string data_base_path(data_root);
		data_base_path.append(data_name);
		
		std::string binary_path(data_base_path);
		binary_path.append(bits_suffixes.at(r_bits));
		binary_path.append(".bin");
		
		binary = load_binary_file(binary_path.c_str());
		arch.address_bits = address_bits;
		arch.r_bits = r_bits;
	}
	
protected:
	
	std::vector<char> binary;
	Arch arch;
};

TEST_P(Translation, succeeds) {
	const uint64_t address = 0x80000000; // TODO Get this from somewhere.
	const Disassembly disassembly = disassemble(binary.data(), binary.size(), address, arch);
	
	std::ostringstream translated;
	ASSERT_TRUE(translate(translated, disassembly, arch));
}

INSTANTIATE_TEST_CASE_P(, Translation, testing::Combine(testing::Values("blr"), testing::Values(b32, b64), testing::Values(b32, b64)));
