#include "cpu/instr.h"
#include "cpu/reg.h"

static void instr_execute() {
	cpu.esp -= data_size / 8;
}

push_REG_helper(push, eax, AX)
push_REG_helper(push, ecx, CX)
push_REG_helper(push, edx, DX)
push_REG_helper(push, ebx, BX)
push_REG_helper(push, esp, SP)
push_REG_helper(push, ebp, BP)
push_REG_helper(push, esi, SI)
push_REG_helper(push, edi, DI)


