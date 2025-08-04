#ifndef BUS
#define BUS
#include<stdint.h>
#include<stdbool.h>
typedef struct bus{
    uint8_t* ram;
} bus_t;

bool init_bus(bus_t* bus);
void clean_bus(bus_t* bus);
bool cpuread(bus_t* bus, uint32_t address, uint8_t* byte);
bool cpuwrite(bus_t* bus, uint32_t address, uint8_t byte);
#endif