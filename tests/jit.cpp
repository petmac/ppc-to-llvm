#include "files.h"

#include "ppc-to-llvm/disassemble.h"
#include "ppc-to-llvm/disassembly.h"
#include "ppc-to-llvm/translate.h"
#include "ppc-to-llvm/translation.h"

#include "gtest/gtest.h"
//#include "llvm/IR/Module.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/Interpreter.h"
//#include "llvm/ExecutionEngine/MCJIT.h"

using namespace llvm;

class JITTests : public testing::TestWithParam<const char *> {
public:
	
	virtual void SetUp() {
		const char *const data_name = GetParam();
		const char *const data_root = "data/translation/";
		std::string data_base_path(data_root);
		data_base_path.append(data_name);
		
		std::string binary_path(data_base_path);
		binary_path.append(".bin");
		
		binary = load_binary_file(binary_path.c_str());
	}
	
protected:
	
	std::vector<char> binary;
};

TEST_P(JITTests, matches_expected) {
	const uint64_t address = 0x80000000; // TODO Get this from somewhere.
	const Disassembly disassembly = disassemble(binary.data(), binary.size(), address);
	Translation translation = translate(disassembly);
	
	ASSERT_NE(translation.run, nullptr);
	
	LLVMInitializeNativeTarget();
	std::string err_str;
	EngineBuilder engine_builder(std::move(translation.module));
	engine_builder.setErrorStr(&err_str);
	std::unique_ptr<ExecutionEngine> engine(engine_builder.create());
	ASSERT_EQ(err_str, "");
	
	engine->runFunction(translation.run, ArrayRef<GenericValue>());
	
	FAIL();
}

INSTANTIATE_TEST_CASE_P(, JITTests, testing::Values("blr"));
