#include "../../../include/cpu.h"

const instruction_t opcodes[256] = {
    DEFINE_INSTRUCTION(0x18, implied, clc, 2),
    DEFINE_INSTRUCTION(0xfb, implied, xce, 2),
    DEFINE_INSTRUCTION(0xa9, immediate, lda, 2),
};



// LDA #const		Load Accumulator from Memory	A9	Immediate	N-----Z-	2[12]	2[1]