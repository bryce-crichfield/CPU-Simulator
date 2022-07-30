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
    RAM ram(0xFFFFFFF);
    Bus bus;

    const word AddressingModeMask = 0b111 << 13;
    const word OperationCodeMask = 0b1111 << 9;
    const word DestinationRegisterMask = 0b111 << 6;
    const word SourceRegister1Mask = 0b111 << 3;
    const word SourceRegister2Mask = 0b111;

    cpu.Connect(bus, true);
    ram.Connect(bus, false);

    // Load
    word load = (AddressingMode::Immediate << 13) | (OperationCode::Load << 9) | (1 << 6);
    word store = (AddressingMode::Direct << 13) | (OperationCode::Store << 9) | (1 << 6);
    // ram.Insert(0b000);

    ram.Insert(load);       // 0x0
    ram.Insert(0x12);       // 0x1
    ram.Insert(store);      // 0x2
    ram.Insert(0x4);        // 0x3

    // load("/home/bryce/Projects/cppsim/test/prg1.o", ram);

    std::cout << "INITIAL STATE" << std::endl;
    cpu.Print();

    while(cpu.flags.halt == 0) {
        cpu.Cycle();
        // std::cout <<"\n" << ed
        cpu.Print();
        std::cin.get();
    }
    return 0;
}