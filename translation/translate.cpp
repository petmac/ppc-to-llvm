#include "ppc-to-llvm/translate.h"

#include "ppc-to-llvm/disassembly.h"
#include "ppc-to-llvm/translation.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"

using namespace llvm;

static Function *generate_run(LLVMContext &context, Module *module) {
	Type *const void_type = Type::getVoidTy(context);
	FunctionType *const function_type = FunctionType::get(void_type, false);
	Function *const run = Function::Create(function_type, GlobalValue::InternalLinkage, "run", module);
	BasicBlock *const entry = BasicBlock::Create(context, "", run);
	IRBuilder<> ir(context);
	ir.SetInsertPoint(entry);
	ir.CreateRetVoid();
	
	return run;
}

static void generate_main(Function *run, LLVMContext &context, Module *module) {
	Type *const void_type = Type::getVoidTy(context);
	FunctionType *const function_type = FunctionType::get(void_type, false);
	Function *const main = Function::Create(function_type, GlobalValue::ExternalLinkage, "main", module);
	BasicBlock *const entry = BasicBlock::Create(context, "", main);
	IRBuilder<> ir(context);
	ir.SetInsertPoint(entry);
	ir.CreateCall(run);
	ir.CreateRetVoid();
}

Translation translate(const Disassembly &disassembly) {
	const std::shared_ptr<LLVMContext> context = std::make_shared<LLVMContext>();
	const std::shared_ptr<Module> module = std::make_shared<Module>("", *context);
	Function *const run = generate_run(*context, module.get());
	generate_main(run, *context, module.get());
	
	Translation translation;
	translation.context = context;
	translation.module = module;
	
	return translation;
}
