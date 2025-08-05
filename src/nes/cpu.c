/**
 * our cpu emulator
 * courtesy of https://wiki.superfamicom.org/uploads/assembly-programming-manual-for-w65c816.pdf
 **/
#include "../../include/cpu.h"


uint8_t fetch(cpu_t* cpu){
    u32 address = JOIN_PC_BNK(cpu);
    uint8_t toreturn = 0;
    bool result = cpuread(cpu->bus, address, &toreturn);
    ASSERT(result, "cpuread failed");
    cpu->pc.raw++;
    return toreturn;
}

/**
 * performs one clock cycle on the cpu
 * @param cpu - reference to cpu structure
 */
void StepCPU(cpu_t* cpu){

    if(cpu->cycles_left == 0){
        uint8_t opcode = fetch(cpu);
        execute_instruction(cpu, opcode);
    }
    cpu->cycles_left--;
}