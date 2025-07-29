#ifndef CPU_H
#define CPU_H
#include<stdint.h>
typedef uint32_t u32;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint64_t u64;
/**
 * a bitfield representation of the 16 bit registers
 * the 65816 registers can either be 16 bit or two 8
 * bit registers
 */
typedef union sreg{
    struct{
        u16 lo: 8;
        u16 hi: 8;
    };
    u16 raw;
} sreg_t;

/**
 * The Program Counter and Index registers have an 
 * additional 8 bits known as the data bank. 
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
    struct{
        u8 ce: 1; // carry + emulation flag
        u8 z: 1;  // zero flag
        u8 i: 1;  // interrupt disable
        u8 d: 1;  // decimal mode
        u8 x: 1;  // index register select
        u8 m: 1;  // memory+accumulator select
        u8 v: 1;  // overflow flag
        u8 n: 1;  // negative flag
    };
    u8 raw;
} status_t;

/**
 * Our CPU struct representation¡
 **/
typedef struct cpu {
    uint64_t cycles_left;
    sreg_t a;
    dreg_t x;
    dreg_t y;
    dreg_t pc;
    u16 d; //direct page register
    status_t flag;
} cpu_t;



#endif