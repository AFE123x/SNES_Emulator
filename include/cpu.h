#ifndef CPU_H
#define CPU_H
#include<stdint.h>
#include<stdbool.h>
#include "common.h"
#include "bus.h"
typedef uint32_t u32;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint64_t u64;
/**
 a bitfield representation of the 16 bit registers the 65816 registers can either be 16 bit or two 8 bit registers
 */
typedef union sdreg{
    struct{
        u16 lo: 8;
        u16 hi: 8;
    };
    u16 raw;
} sreg_t;

/**
    The Program Counter and Index registers have an additional 8 bits known as the data bank.
 */
typedef union dreg{
    struct{
        u32 lo: 8;
        u32 hi: 8;
        u32 bank: 8;
    };
    u32 raw;
} dreg_t;

/**
 * our status register representation
 **/
typedef union status{
    struct native{
        u8 ce: 1; // carry + emulation flag
        u8 z: 1;  // zero flag
        u8 i: 1;  // interrupt disable
        u8 d: 1;  // decimal mode
        u8 x: 1;  // index register select
        u8 m: 1;  // memory+accumulator select
        u8 v: 1;  // overflow flag
        u8 n: 1;  // negative flag
    } n;
    struct emulation{
        u8 ce: 1;
        u8 z: 1;
        u8 i: 1;
        u8 d: 1;
        u8 b: 1;
        u8 u: 1; //unused
        u8 v: 1;
        u8 n: 1;
    } e;
    u8 raw;
} status_t;

/**
 * Our CPU struct representation¡
 **/
typedef struct cpu {
    u64 cycles_left;
    sreg_t a;
    dreg_t x;
    dreg_t y;
    dreg_t pc; // includes program bank register
    u16 d; // direct page register
    status_t flag;
    u32 address; //calculating address from addressing mode
    u16 data; //data we read from address
    bool emulation_mode;
    bus_t* bus;
} cpu_t;


void StepCPU(cpu_t* cpu);

typedef struct instruction{
    void (*addressing_mode)(cpu_t* cpu);
    void (*instruction)(cpu_t* cpu);
    u8 cycles_left;
}instruction_t;

/* Helper macro to define elements in opcode table*/
#define DEFINE_INSTRUCTION(opcode,addressmode,instruct,cycles) \
[opcode] = { \
    .addressing_mode = addressmode, \
    .instruction = instruct, \
    .cycles_left = cycles, \
}

/* Addressing modes */
void implied(cpu_t* cpu);

/* Instructions*/
void clc(cpu_t* cpu);  void xce(cpu_t* cpu);

/* unit tests */
#ifdef TEST
void testcpu();
#endif
#endif