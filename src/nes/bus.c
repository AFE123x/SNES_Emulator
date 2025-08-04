#include "../../include/bus.h"
#include<stdlib.h>

bool cpuread(bus_t* bus, uint32_t address, uint8_t* byte){
    *byte = bus->ram[address];
    return true;
}

bool cpuwrite(bus_t* bus, uint32_t address, uint8_t byte){
    bus->ram[address] = byte;
    return true;
}

/**
 * Initializes the bus, allocating the ram needed.
 * @param bus - reference to the bus structure
 * @return true if initialization succeeds, false if fails.
 */
bool init_bus(bus_t* bus){
    bus->ram = (uint8_t*)malloc(sizeof(uint8_t) * 16777216);
    if(bus->ram == NULL) return false;
    return true;
}

/**
 * Free's the ram and other heap allocated items in bus.
 * @param bus - a reference to the bus structure
 */
void clean_bus(bus_t* bus){
    free(bus->ram);
}