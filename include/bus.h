#ifndef BUS
#define BUS
#include<stdint.h>
#include<stdbool.h>
typedef struct bus{
    uint8_t* ram;
} bus_t;


bool cpuread(bus_t* bus, uint32_t address, uint8_t* byte);
bool cpuwrite(bus_t* bus, uint32_t address, uint8_t byte);
#endif