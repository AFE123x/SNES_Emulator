#include "../../../include/cpu.h"

const instruction_t opcodes[256] = {
    DEFINE_INSTRUCTION(0x18, implied, clc, 2),
    DEFINE_INSTRUCTION(0xfb, implied, xce, 2),
};



