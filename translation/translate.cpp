#include "ppc-to-llvm/translate.h"

#include "ppc-to-llvm/disassembly.h"
#include "ppc-to-llvm/translation.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"

Translation translate(const Disassembly &disassembly) {
	Translation translation;
	translation.context = std::make_shared<llvm::LLVMContext>();
	llvm::LLVMContext &context = *translation.context;
	translation.module = std::make_shared<llvm::Module>("", context);
	llvm::Type *const void_type = llvm::Type::getVoidTy(context);
	llvm::FunctionType *const function_type = llvm::FunctionType::get(void_type, false);
	llvm::Function *const main = llvm::Function::Create(function_type, llvm::GlobalValue::InternalLinkage, "main", translation.module.get());
	llvm::BasicBlock *const entry = llvm::BasicBlock::Create(context, "", main);
	llvm::IRBuilder<> ir(context);
	ir.SetInsertPoint(entry);
	ir.CreateRetVoid();
	
	return translation;
}
