#include "../../../include/cpu.h"

void implied(cpu_t* cpu){
    /* nothing to do here */

}

void immediate(cpu_t* cpu){
    if(cpu->flag.e.ce){
        //we're in native mode.
        u32 lo = (u32)fetch(cpu);
        u32 hi = (u32)fetch(cpu);
        cpu->address = (cpu->data_bank_register << 16) | (hi << 8) | lo;
    }
}
