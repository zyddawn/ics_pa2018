#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

#define push_helper(data) \
	cpu.esp -= 4; \
	vaddr_write(cpu.esp, SREG_SS, 4, data);

void raise_intr(uint8_t intr_no) {
#ifdef IA32_INTR
	// printf("Please implement raise_intr()");
	// assert(0);
	
	// push EFLAGS, CS, EIP
	push_helper(cpu.eflags.val)
	push_helper((uint32_t) cpu.cs.val)
	push_helper(cpu.eip)

	// Find IDT with "intr_no"
	laddr_t idt_addr = (laddr_t)cpu.idtr.base | (intr_no << 3);
	// printf("idtr = 0x%08x, intr_no = 0x%x, gd_addr = 0x%x\n", cpu.idtr.base, intr_no, gd_addr);

	GateDesc idt;
	idt.val[0] = laddr_read((laddr_t)(idt_addr), 4);
	// printf("idt[0] = 0x%x\n", idt.val[0]);
	idt.val[1] = laddr_read((laddr_t)(idt_addr + 4), 4);
	// printf("idt[1] = 0x%x\n", idt.val[1]);

	assert(idt.present == 1);
	// clear IF if it's interrupt gate (type == 0xE)
	if (idt.type == 0xE)
		cpu.eflags.IF = 0;

	// set eip to the entry of interrupt handler
	uint32_t new_eip = (idt.offset_15_0 & 0xffff) | ((idt.offset_31_16 & 0xffff) << 16),
		 new_cs = idt.selector;
	cpu.eip = new_eip;  // update eip
	cpu.cs.val = new_cs;
	load_sreg(1);	// reload CS
	
#endif
}

void raise_sw_intr(uint8_t intr_no) {
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
