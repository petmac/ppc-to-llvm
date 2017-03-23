#include "ppc-to-llvm/translate.h"

#include "ppc-to-llvm/disassembly.h"
#include "ppc-to-llvm/translation.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"

using namespace llvm;

static void generate_main(LLVMContext &context, Module *module) {
	Type *const void_type = Type::getVoidTy(context);
	FunctionType *const function_type = FunctionType::get(void_type, false);
	Function *const main = Function::Create(function_type, GlobalValue::InternalLinkage, "main", module);
	BasicBlock *const entry = BasicBlock::Create(context, "", main);
	IRBuilder<> ir(context);
	ir.SetInsertPoint(entry);
	ir.CreateRetVoid();
}

Translation translate(const Disassembly &disassembly) {
	const std::shared_ptr<LLVMContext> context = std::make_shared<LLVMContext>();
	const std::shared_ptr<Module> module = std::make_shared<Module>("", *context);
	generate_main(*context, module.get());
	
	Translation translation;
	translation.context = context;
	translation.module = module;
	
	return translation;
}
