#include "op.h"

static void out_reg_ref(std::ostream &out, ppc_reg reg, const char *arch_type) {
	out << arch_type << "* %r" << reg - PPC_REG_R0;
}

static void out_reg_ref(std::ostream &out, const cs_ppc_op &operand, const char *arch_type) {
	out_reg_ref(out, static_cast<ppc_reg>(operand.reg), arch_type);
}

OP_FN(LI) {
	assert(op_count == 2);
	assert(operands[0].type == PPC_OP_REG);
	assert(operands[1].type == PPC_OP_IMM);
	
	out << "store " << arch_type << " " << operands[1].imm << ", ";
	out_reg_ref(out, operands[0], arch_type);
	out << std::endl;
}
