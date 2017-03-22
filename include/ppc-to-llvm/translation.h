#pragma once

#include <memory>

namespace llvm {
	class LLVMContext;
	class Module;
}

struct Translation {
	std::shared_ptr<llvm::LLVMContext> context;
	std::shared_ptr<llvm::Module> module;
};
