#include "../../../include/cpu.h"
#include <stdio.h>

void execute_instruction(cpu_t *cpu, uint8_t opcode) {
    bool prop = 0;
    uint8_t byte;
    uint16_t lo = 0;
    uint16_t hi = 0;
    uint8_t hibyte, lowbyte = 0;
    switch (opcode) {
    case 0x78: // SEI Instruction
        cpu->flag.n.i = 1;
        cpu->cycles_left = 2;
        break;

    case 0x18: // CLC instruction
        cpu->flag.n.ce = 0;
        cpu->cycles_left = 2;
        break;

    case 0xfb: // XCE instruction
        prop = cpu->flag.n.ce > 0;
        cpu->flag.n.ce = cpu->emulation_mode;
        cpu->emulation_mode = prop;
        cpu->cycles_left = 2;

        if (cpu->emulation_mode) {
            // Entering emulation mode
            // Force M and X flags to 1 (8-bit mode)
            cpu->flag.n.m = 1;
            cpu->flag.n.x = 1;

            // Truncate index registers to 8-bit
            cpu->x.hi = 0;
            cpu->y.hi = 0;

            // Set stack pointer high byte to 0x01 (page 1)
            cpu->sp = (cpu->sp & 0xFF) | 0x0100;
        }
        break;

    case 0xa9: // LDA(IMM) instruction
        // Check if we're in native mode AND accumulator is 16-bit
        if (!cpu->emulation_mode && !cpu->flag.n.m) {
            // 16-bit accumulator mode
            lo = fetch(cpu);
            hi = fetch(cpu);
            cpu->a.hi = hi;
            cpu->a.lo = lo;
            cpu->flag.n.n = (cpu->a.raw & 0x8000) > 0; // MSB is 1
            cpu->flag.n.z = cpu->a.raw == 0;
            cpu->cycles_left = 3;
        } else {
            // 8-bit accumulator mode
            byte = fetch(cpu);
            cpu->a.lo = byte;
            cpu->flag.n.n = (cpu->a.lo & 0x80) > 0; // MSB is 1
            cpu->flag.n.z = cpu->a.lo == 0;
            cpu->cycles_left = 2;
        }
        break;
    case 0xeb: // XBA Instruction
        // Swap accumulator bytes using XOR
        cpu->a.hi = cpu->a.hi ^ cpu->a.lo;
        cpu->a.lo = cpu->a.hi ^ cpu->a.lo;
        cpu->a.hi = cpu->a.hi ^ cpu->a.lo;
        // Update flags based on new low byte
        cpu->flag.n.n = (cpu->a.lo & 0x80) > 0;
        cpu->flag.n.z = cpu->a.lo == 0;
        cpu->cycles_left = 3;
        break;

    case 0xe2: // SEP instruction
        byte = fetch(cpu);
        cpu->flag.raw |= byte;
        cpu->cycles_left = 3;
        if (cpu->flag.n.x) {
            cpu->x.hi = cpu->y.hi = 0;
        }
        break;

    case 0xc2: // REP instruction
        byte = fetch(cpu);
        cpu->flag.raw &= ~byte;
        cpu->cycles_left = 3;
        if (cpu->flag.n.x) {
            cpu->x.hi = cpu->y.hi = 0;
        }
        break;
    case 0xad: // LDA(ABS)

        lo = fetch(cpu);
        hi = fetch(cpu);
        cpu->address = (hi << 8) | lo;
        cpu->address = (cpu->data_bank_register << 16) | (cpu->address & 0xFFFF);
        if (!cpu->emulation_mode && !cpu->flag.n.m) {

            cpuread(cpu->bus, cpu->address, &lowbyte);
            cpuread(cpu->bus, cpu->address + 1, &hibyte);
            cpu->a.hi = hibyte;
            cpu->a.lo = lowbyte;
            cpu->flag.n.n = (cpu->a.raw & 0x8000) > 0; // MSB is 1
            cpu->flag.n.z = cpu->a.raw == 0;
            cpu->cycles_left = 3;
        } else {
            // 8-bit accumulator mode
            cpuread(cpu->bus, cpu->address, &lowbyte);
            cpu->a.lo = lowbyte;
            cpu->flag.n.n = (cpu->a.lo & 0x80) > 0; // MSB is 1
            cpu->flag.n.z = cpu->a.lo == 0;
            cpu->cycles_left = 2;
        }
        cpu->cycles_left = 4;
        if (cpu->flag.n.m == 0)
            cpu->cycles_left++;
        break;
    default:
        TODO("instruction not implemented yet");
    }
}