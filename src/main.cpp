#include <iostream>
#include "def/def.hpp"
#include "cpu/cpu.hpp"
#include "ram/ram.hpp"
#include "bus/bus.hpp"
#include "loader/loader.hpp"

int main(void) 
{
    // SysConfig config;
    // config.MEM_SIZE     = 0xFFFF;
    // config.PRG_START    = 0xFFFF;
    // config.PRG_END      = 0xFFFF - 0x100;
    // config.STK_START    = 0x0000;
    // config.STK_END      = 0x0000 + 0x100;

    CPU cpu;
    cpu.Reset();
    RAM ram(256);
    Bus bus;

    cpu.Connect(bus, true);
    ram.Connect(bus, false);

    // load("/home/bryce/Projects/cppsim/test/prg1.o", ram);

    while(true) {
        cpu.Print();
        cpu.Cycle();
        std::cin.get();
    }
    return 0;
}