#include "cpu.hpp"

CPU::CPU()
    : 
    addressing_unit(*(new AddressingUnit(*this))),
    control_unit(*(new ControlUnit(*this))),
    arithmetic_logic_unit(*(new ArithmeticLogicUnit(*this))),
    flags(*(new Flags()))
{
    return;
}

CPU::~CPU()
{
}

void CPU::Read()
{
    addressing_unit.Read();
}

void CPU::Write()
{
    addressing_unit.Write();
}

void CPU::Reset()
{
    return;
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