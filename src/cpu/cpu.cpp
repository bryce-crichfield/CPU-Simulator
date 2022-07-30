#include "cpu.hpp"
#include <string>

std::string PrintAddressingMode(AddressingMode mode)
{
    switch (mode)
    {
        case Immediate:
            return "Immediate";
            break;
        case Direct:
            return "Direct";
            break;
        case Indirect:
            return "Indirect";
            break;
        case SingleInternal:
            return "SingleInternal";
            break;
        case DualInternal:
            return "DualInternal";
            break;
        default:
            return "Unrecognized";
            break;
    }
    return "Print Error";
}

std::string PrintOperationCode(OperationCode code)
{
    switch (code)
    {
        case Noop:
            return "Noop";
            break;
        case Load:
            return "Load";
            break;
        case Store:
            return "Store";
            break;
        case Jump:
            return "Jump";
            break;
        case Add:
            return "Add";
            break;
        case Sub:
            return "Sub";
            break;
        case Multiply:
            return "Multiply";
            break;
        case Halt:
            return "Halt";
            break;
        default:
            return "Unrecognized";
            break;
    }
    return "Print Error";
}

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
    // while (flags.halt == false)
    // {
        addressing_unit.Fetch();
        control_unit.Decode();
        arithmetic_logic_unit.Execute();
        if (flags.error)
        {
            printf("Error Flag Raised\nHalting CPU\n");
            flags.halt = true;
        }
    // }
}

void CPU::Print()
{
    using namespace std;
    cout << "CPU Report -----" << endl;
    addressing_unit.Print();
    control_unit.Print();
    arithmetic_logic_unit.Print();
}