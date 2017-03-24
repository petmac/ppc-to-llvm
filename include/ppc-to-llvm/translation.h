#pragma once

#include <memory>

namespace llvm {
	class Function;
	class LLVMContext;
	class Module;
}

struct Translation {
	std::shared_ptr<llvm::LLVMContext> context;
	std::unique_ptr<llvm::Module> module;
	llvm::Function *run = nullptr;
};
