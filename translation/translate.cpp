#include "ppc-to-llvm/translate.h"

#include "ppc-to-llvm/disassembly.h"
#include "ppc-to-llvm/translation.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"

using namespace llvm;

Translation translate(const Disassembly &disassembly) {
	Translation translation;
	translation.context = std::make_shared<LLVMContext>();
	LLVMContext &context = *translation.context;
	translation.module = std::make_shared<Module>("", context);
	Type *const void_type = Type::getVoidTy(context);
	FunctionType *const function_type = FunctionType::get(void_type, false);
	Function *const main = Function::Create(function_type, GlobalValue::InternalLinkage, "main", translation.module.get());
	BasicBlock *const entry = BasicBlock::Create(context, "", main);
	IRBuilder<> ir(context);
	ir.SetInsertPoint(entry);
	ir.CreateRetVoid();
	
	return translation;
}
