#include "../include/nes.h"
#include "../include/cpu.h"
#include <stdbool.h>
#include <stdio.h>
#include<assert.h>


/**
 * performs one clock tick
 * @param snes - the system we want to perform clock tick on.
 */
void clock(snes_t* snes){
    StepCPU(snes->cpu);
    
}
