
#include "../include/nes.h"
#include<string.h>
#include<stdio.h>
#include<cjson/cJSON.h>
#include<assert.h>
#include<stdlib.h>


/**
 * main - where our program starts
 * @param argc - number of command line arguments
 * @param argv - vector of arguments
 */
int main(int argc, char const *argv[])
{
    #ifndef TEST
        cpu_t cpu;
        memset(&cpu,0,sizeof(cpu_t)); //clear cpu_t type to defined state.
        cpu.flag.e.ce = 1; //system's initially in emulation mode.
        DEFINE_SNES(snes,cpu) //macro to declare and define snes_t struct
        clock(&snes);
    #else
        testcpu();
    #endif


    return 0;
}


