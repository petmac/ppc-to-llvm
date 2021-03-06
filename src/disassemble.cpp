#include "ppc-to-llvm/ppc-to-llvm.h"

#include <capstone/capstone.h>

#include <functional>
#include <map>

static const std::map<Bits, cs_mode> bits_to_mode = {
	{ b32, CS_MODE_32 },
	{ b64, CS_MODE_64 },
};

static void close_capstone(csh *csp) {
	cs_close(csp);
	delete csp;
}

Disassembly disassemble(const void *binary, size_t binary_size, uint64_t address, const Arch &arch) {
	const Capstone cs(new csh(0), &close_capstone);
	const cs_mode mode = static_cast<cs_mode>(bits_to_mode.at(arch.arch_bits) | CS_MODE_BIG_ENDIAN);
	cs_err err = cs_open(CS_ARCH_PPC, mode, cs.get());
	if (err != CS_ERR_OK) {
		return Disassembly();
	}
	
	err = cs_option(*cs, CS_OPT_DETAIL, CS_OPT_ON);
	if (err != CS_ERR_OK) {
		return Disassembly();
	}
	
	cs_insn *unsafe_insn = nullptr;
	const size_t insn_count = cs_disasm(*cs, static_cast<const uint8_t *>(binary), binary_size, address, 0, &unsafe_insn);
	const std::function<void(cs_insn *)> free_instructions = [insn_count](cs_insn *insn) {
		cs_free(insn, insn_count);
	};
	const Instructions insn(unsafe_insn, free_instructions);
	
	Disassembly disassembly;
	disassembly.cs = cs;
	disassembly.insn = insn;
	disassembly.insn_count = insn_count;
	
	return disassembly;
}
