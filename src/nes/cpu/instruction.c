#include "../../../include/cpu.h"
#include<stdio.h>

void execute_instruction(cpu_t* cpu,uint8_t opcode){
    printf("opcode 0x%x\n",opcode);
    bool prop = 0;
    uint8_t byte;
    switch(opcode){
        case 0x78: //SEI Instruction
            cpu->flag.n.i = 1;
            cpu->cycles_left = 2;
            break;
        case 0x18: //CLC instruction
            cpu->flag.n.ce = 0;
            cpu->cycles_left = 2;
            break;
        case 0xfb: //XCE instruction 
            prop = cpu->flag.n.ce > 0;
            cpu->flag.n.ce = cpu->emulation_mode;
            cpu->emulation_mode = prop;
            cpu->cycles_left = 2;
            break;
        case 0xa9: //LDA(IMM) instruction

            //first, check if we're working with tha 16 bit accumulator
            if(cpu->flag.n.ce && !cpu->flag.n.m){
                //this means we're reading 2 bytes, and the accumulator is 2 bytes
                uint8_t lo = fetch(cpu);
                uint8_t hi = fetch(cpu);
                cpu->a.hi = hi;
                cpu->a.lo = lo;
                cpu->flag.n.n = (cpu->a.raw & 0x80 ) > 0; //MSB is 1
                cpu->flag.n.z = cpu->a.raw == 0;
                cpu->cycles_left = 3;
            }
            else{
                uint8_t byte = fetch(cpu);
                cpu->a.lo = byte;
                cpu->flag.n.n = (cpu->a.lo & 0x80 ) > 0; //MSB is 1
                cpu->flag.n.z = cpu->a.lo == 0;
                cpu->cycles_left = 2;
            }
            break;
        case 0xeb: //XBA Instruction
            cpu->a.hi = cpu->a.hi ^ cpu->a.lo;
            cpu->a.lo = cpu->a.hi ^ cpu->a.lo;
            cpu->a.hi = cpu->a.hi ^ cpu->a.lo;
            break;
        case 0xe2: //SEP instruction
            byte = fetch(cpu);
            cpu->flag.raw |= byte;
            cpu->cycles_left = 3;
            break;
        case 0xc2: //REP instruction
            byte = fetch(cpu);
            cpu->flag.raw &= ~byte;
            cpu->cycles_left = 3;
            break;
            break;
        default:
            TODO("instruction not implemented yet")
    };
}

// x = x xor y
// y = x xor y
// x = x xor y