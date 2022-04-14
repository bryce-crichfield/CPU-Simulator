#include "cpu.hpp"

CPU::CPU()
    : addressing_unit(*(new AddressingUnit(*this))),
      control_unit(*(new ControlUnit(*this))),
      arithmetic_logic_unit(*(new ArithmeticLogicUnit(*this))),
      flags(*(new Flags()))
{
}

CPU::~CPU()
{
}

void CPU::Reset()
{
}

void CPU::Cycle()
{
    while (flags.halt == false)
    {
        addressing_unit.Fetch();
        control_unit.Decode();
        arithmetic_logic_unit.Execute();
        if (flags.error)
        {
            printf("Error Flag Raised\nHalting CPU\n");
            flags.halt = true;
        }
    }
}

void CPU::Print()
{
    using namespace std;
    cout << "CPU Report -----" << endl;
    addressing_unit.Print();
    control_unit.Print();
    arithmetic_logic_unit.Print();
}