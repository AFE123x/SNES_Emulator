#include "../../../include/cpu.h"
#include<stdio.h>

/**
 * @brief Test the SEI, CLC, XCE, SEP, and LDA instruction
 * @param cpu - cpu reference
 * @result will fail if interrupt disable flag isn't enabled
 */
static void test1(cpu_t* cpu){
    uint8_t byte[10] = {0x78, 0x18, 0xfb, 0xe2, 0x20, 0xc2, 0x20, 0xa9, 0xad, 0xde};
/*

Addr    Opcode Instruction                              Cycles
------- ------ ----------------------------------       ------
0x0000: 0x78 sei                                      ?
0x0001: 0x18 clc                                      ?
0x0002: 0xfb xce                                      ?
0x0003: 0xe2 sep #0x20                                ?
0x0004: 0x20
0x0005: 0xc2 rep #0x20                                ?
0x0006: 0x20
0x0007: 0xa9 lda #0xdead                              ?
0x0008: 0xad
0x0009: 0xde
*/
    cpu->emulation_mode = true;
    for(int i = 0; i < 10; i++){
        cpuwrite(cpu->bus,i,byte[i]);
    }
    cpu->pc.raw = 0;
    cpu->a.raw = 0;
    cpu->flag.raw = 0;
    cpu->cycles_left = 0;
    StepCPU(cpu);
    ASSERT(cpu->flag.n.i,"test1 - SEI failed\n");

    ASSERT(cpu->cycles_left == 1, "test1 - SEI cycles left incorrect");
    cpu->cycles_left = 0;
    cpu->flag.n.ce = 1;
    /* Test CLC instruction */
    StepCPU(cpu);
    ASSERT(!cpu->flag.n.ce, "test1 - CLC instruction failed");
    ASSERT(cpu->cycles_left == 1, "test1 - CLC cycles left incorrect");
    cpu->cycles_left = 0;

    /* test XCE instruction*/
    
    StepCPU(cpu);
    ASSERT(!cpu->emulation_mode,"XCE Instruction - emulation flag failed");
    ASSERT(cpu->flag.n.ce,"XCE Instruction - carry flag falied!")

    /* Test SEP(IMM) instruction */
    cpu->cycles_left = 0;
    StepCPU(cpu);
    ASSERT(cpu->flag.raw & 0x20, "SEP instruction - FAILED TO SET BIT");
    /* Test REP(IMM) Instruction */
    cpu->cycles_left = 0;
    StepCPU(cpu);
    ASSERT(!(cpu->flag.raw & 0x20),"REP instruction - FAILED TO RESET BIT")
    /* LDA(IMM) */
    cpu->cycles_left = 0;
    StepCPU(cpu);
    ASSERT(cpu->a.raw == 0xDEAD, "LDA(IMM) test - A is not 0xDEAD, FAILED!")
}
/**
 * @brief Tests the CPU's CLC instruction
 * @param cpu - cpu reference
 * @result will fail if carry flag isn't cleared
 */
static void test2(cpu_t* cpu){

}

void (*test_ptr[256])(cpu_t* cpu) = {
    test1,
    test2,
};

void test(cpu_t* cpu){
    for(int i = 0; i < 2; i++){
        test_ptr[i](cpu);
    }
    printf("test succeed\n");
}