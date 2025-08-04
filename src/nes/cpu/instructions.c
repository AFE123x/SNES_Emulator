#include "../../../include/cpu.h"

/**
 * clear the Carry Flag (same regardless of emulation mode)
 * @param cpu - our cpu implementation
 */
void clc(cpu_t* cpu){
    cpu->flag.n.ce = 0;
}

void xce(cpu_t* cpu){
    cpu->emulation_mode = cpu->flag.n.ce > 0;
}

void lda(cpu_t* cpu){
    
}