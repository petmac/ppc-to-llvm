#include "gtest/gtest.h"

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

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

class Translation : public testing::TestWithParam<const char *> {
public:
	
	virtual void SetUp() {
		const char *const data_name = GetParam();
		const char *const data_root = "data/";
		std::string data_base_path(data_root);
		data_base_path.append(data_name);
		
		std::string binary_path(data_base_path);
		binary_path.append(".bin");
		
		std::string expected_ll_path(data_base_path);
		expected_ll_path.append(".ll");
		
		binary = load_binary_file(binary_path.c_str());
	}
	
protected:
	
	std::vector<char> binary;
};

TEST_P(Translation, matches_expected) {
//	std::cout << binary.data();
}

INSTANTIATE_TEST_CASE_P(, Translation, testing::Values("blr"));

int main(int argc, char *argv[]) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
