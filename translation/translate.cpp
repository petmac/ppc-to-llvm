#include "ppc-to-llvm/translate.h"

#include "ppc-to-llvm/disassembly.h"
#include "ppc-to-llvm/translation.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"

using namespace llvm;

static StructType *generate_state_type(Module *module) {
	LLVMContext &context = module->getContext();
	return StructType::create("State", Type::getInt32Ty(context), nullptr);
}

static Function *generate_run(StructType *state_type, Module *module) {
	LLVMContext &context = module->getContext();
	Type *const void_type = Type::getVoidTy(context);
	Type *const state_ptr_type = PointerType::getUnqual(state_type);
	FunctionType *const function_type = FunctionType::get(void_type, state_ptr_type, false);
	Function *const run = Function::Create(function_type, GlobalValue::ExternalLinkage, "run", module);
	run->setDLLStorageClass(GlobalValue::DLLExportStorageClass);
	Argument *const state_arg = &*run->args().begin();
	state_arg->setName("state");
	BasicBlock *const entry = BasicBlock::Create(context, "", run);
	IRBuilder<> ir(context);
	ir.SetInsertPoint(entry);
	Value *const r0 = ir.CreateStructGEP(Type::getInt32Ty(context), state_arg, 0);
	ir.CreateRetVoid();
	
	return run;
}

static void generate_main(Function *run, Module *module) {
	LLVMContext &context = module->getContext();
	Type *const void_type = Type::getVoidTy(context);
	FunctionType *const function_type = FunctionType::get(void_type, false);
	Function *const main = Function::Create(function_type, GlobalValue::ExternalLinkage, "main", module);
	BasicBlock *const entry = BasicBlock::Create(context, "", main);
	IRBuilder<> ir(context);
	ir.SetInsertPoint(entry);
	ir.CreateCall(run, ConstantPointerNull::get(Type::getInt32PtrTy(context)));
	ir.CreateRetVoid();
}

Translation translate(const Disassembly &disassembly) {
	const std::shared_ptr<LLVMContext> context = std::make_shared<LLVMContext>();
	const std::shared_ptr<Module> module = std::make_shared<Module>("", *context);
	StructType *const state_type = generate_state_type(module.get());
	Function *const run = generate_run(state_type, module.get());
	generate_main(run, module.get());
	
	Translation translation;
	translation.context = context;
	translation.module = module;
	
	return translation;
}
