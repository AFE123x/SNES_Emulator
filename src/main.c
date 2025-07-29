
#include "../include/nes.h"
#include<string.h>
/**
 * main - where our program starts
 * @param argc - number of command line arguments
 * @param argv - vector of arguments
 */
int main(int argc, char const *argv[])
{
    cpu_t cpu;
    memset(&cpu,0,sizeof(cpu_t)); //clear cpu_t type to defined state.
    DEFINE_SNES(snes,cpu) //macro to declare and define snes_t struct

    clock(&snes);

    return 0;
}
