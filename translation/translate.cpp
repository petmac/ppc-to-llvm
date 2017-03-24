#include "ppc-to-llvm/translate.h"

#include "ppc-to-llvm/disassembly.h"
#include "ppc-to-llvm/translation.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"

using namespace llvm;

static Type *generate_state(LLVMContext &context) {
	return StructType::create(context, "State");
}

static Function *generate_run(Type *state, Module *module) {
	LLVMContext &context = module->getContext();
	Type *const void_type = Type::getVoidTy(context);
	const std::initializer_list<Type *> args = { state };
	FunctionType *const function_type = FunctionType::get(void_type, args, false);
	Function *const run = Function::Create(function_type, GlobalValue::InternalLinkage, "run", module);
	Function::ArgumentListType &fun_args = run->getArgumentList();
	Argument &state_arg = fun_args.front();
	state_arg.setName("state");
	BasicBlock *const entry = BasicBlock::Create(context, "", run);
	IRBuilder<> ir(context);
	ir.SetInsertPoint(entry);
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
	ir.CreateCall(run);
	ir.CreateRetVoid();
}

Translation translate(const Disassembly &disassembly) {
	const std::shared_ptr<LLVMContext> context = std::make_shared<LLVMContext>();
	Type *const state = generate_state(*context);
	std::unique_ptr<Module> module = make_unique<Module>("", *context);
	Function *const run = generate_run(state, module.get());
	generate_main(run, module.get());
	
	Translation translation;
	translation.context = context;
	translation.module = std::move(module);
	translation.run = run;
	
	return translation;
}
