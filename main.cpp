#include <iostream>
#include "inc/def.hpp"
#include "inc/cpu.hpp"
#include "inc/ram.hpp"
#include "inc/bus.hpp"

int main(void) 
{
    SysConfig config;
    config.MEM_SIZE     = 0xFFFF;
    config.PRG_START    = 0xFFFF;
    config.PRG_END      = 0xFFFF - 0x100;
    config.STK_START    = 0x0000;
    config.STK_END      = 0x0000 + 0x100;

    CPU cpu(config);
    cpu.Reset();
    RAM ram(config);
    Bus bus;

    cpu.Connect(bus, true);
    ram.Connect(bus, false);

    ram.Insert(0x00);
    ram.Insert(0x1);
    ram.Insert(0x00);
    ram.Insert(0x2);
    ram.Insert(0x00);
    ram.Insert(0x3);

    while(true) {
        cpu.Print();
        cpu.Cycle();
        std::cin.get();
    }
    return 0;
}