
#include "../include/nes.h"
#include "../include/bus.h"
#include<string.h>
#include<stdio.h>
#include<assert.h>
#include<stdlib.h>


/**
 * main - where our program starts
 * @param argc - number of command line arguments
 * @param argv - vector of arguments
 */
int main(/* int argc, char const *argv[] */)
{
    cpu_t cpu;
    memset(&cpu,0,sizeof(cpu));
    DEFINE_SNES(snes, cpu)
    bus_t bus;
    ASSERT(init_bus(&bus), "Failed to initialize bus")
    cpu.bus = &bus;
    clock(&snes);

    clean_bus(&bus);
    return 0;
}


