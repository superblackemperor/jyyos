#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(auipc) f(jal) f(jalr) \
	f(beq) f(bne) f(bge) f(blt)/*B jump*/ \
	f(lw) f(lbu)/*I load*/ \
	f(sw) f(sh) f(sb)/*S store*/  \
	f(addi) f(ani) f(sltiu) f(srai) f(srli) f(xori)/*I arith_imm*/ \
        f(add) f(sub) f(sltu) f(xor) f(or) f(sll) f(slt) f(and) \
	f(mul) f(div) f(rem)/*R arith_reg*/ \
	f(inv) f(nemu_trap)

def_all_EXEC_ID();
