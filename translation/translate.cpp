#include "ppc-to-llvm/translate.h"

#include "ppc-to-llvm/disassembly.h"
#include "ppc-to-llvm/translation.h"

#include "llvm/IR/Module.h"

Translation translate(const Disassembly &disassembly) {
	Translation translation;
	translation.context = std::make_shared<llvm::LLVMContext>();
	translation.module = std::make_shared<llvm::Module>("module", *translation.context);
	
	return translation;
}
