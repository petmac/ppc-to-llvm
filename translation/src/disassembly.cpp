#include "disassembly.h"

#include <capstone/capstone.h>

static void close_capstone(csh *csp) {
	cs_close(csp);
	delete csp;
}

Disassembly disassemble(const void *binary, size_t binary_size) {
	Disassembly disassembly;
	disassembly.cs = Capstone(new csh(0), &close_capstone);
	
	const cs_mode mode = static_cast<cs_mode>(CS_MODE_64 | CS_MODE_BIG_ENDIAN);
	const cs_err err = cs_open(CS_ARCH_PPC, mode, disassembly.cs.get());
	if (err != CS_ERR_OK) {
		return Disassembly();
	}
	
	return disassembly;
}
