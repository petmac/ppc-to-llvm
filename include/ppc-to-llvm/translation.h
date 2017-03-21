#pragma once

#include <memory>

namespace llvm {
	class LLVMContext;
}

struct Translation {
	std::unique_ptr<llvm::LLVMContext> context;
};
