#ifndef NES_H
#define NES_H
#include "cpu.h"
typedef struct snes{
    cpu_t* cpu;
} snes_t;


/**
 * Declaration macro to declare and define the nes_t type
 */
#define DEFINE_SNES(name,cpu) snes_t name = { \
    .cpu = &cpu \
};


void clock(snes_t* nes);

#endif