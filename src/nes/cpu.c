/**
 * our cpu emulator
 * courtesy of https://wiki.superfamicom.org/uploads/assembly-programming-manual-for-w65c816.pdf
 **/
#include "../../include/cpu.h"
#include "../../include/common.h"
/**
 * performs one clock cycle on the cpu
 * @param cpu - reference to cpu structure
 */
void StepCPU(cpu_t* cpu){

    if(cpu->cycles_left == 0){
        TODO()
    }
}