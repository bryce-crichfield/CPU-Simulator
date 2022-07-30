#include "cpu.hpp"
#include <stdio.h>

ArithmeticLogicUnit::ArithmeticLogicUnit(CPU &cpu) : SubUnit(cpu)
{
    r0 = r1 = r2 = r3 = r4 = r5 = r6 = 0;
}

void ArithmeticLogicUnit::Execute()
{
    // Todo: this could be easily eliminated by storing an inter-
    // class functinon pointer in instead of an enum that will have
    // to be rechecked
    switch (cpu.control_unit.ocr)
    {
    case OperationCode::Noop:
        Noop();
        break;
    case OperationCode::Load:
        Load();
        break;
    case OperationCode::Store:
        Store();
        break;
    case OperationCode::Jump:
        Jump();
        break;
    case OperationCode::Halt:
        Halt();
        break;
    }
}

void ArithmeticLogicUnit::Noop()
{
    return;
}

void ArithmeticLogicUnit::Load()
{
    *cpu.control_unit.destination_register = cpu.addressing_unit.mdr;
}

void ArithmeticLogicUnit::Store()
{
    cpu.addressing_unit.mdr = *cpu.control_unit.destination_register;
    // Force Write Out Here?
    cpu.Write();
    // But what if we do
}

void ArithmeticLogicUnit::Jump()
{
    cpu.control_unit.prc = cpu.addressing_unit.mar;
}

void ArithmeticLogicUnit::Halt()
{
    cpu.flags.halt = true;
}

void ArithmeticLogicUnit::Print()
{
    using namespace std;
    cout << "----- Arithmetic Logic Unit -----" << endl;
    cout << "----- --- R0 = " << r1 << endl;
    cout << "----- --- R1 = " << r2 << endl;
    cout << "----- --- R2 = " << r2 << endl;
    cout << "----- --- R3 = " << r3 << endl;
    cout << "----- --- R4 = " << r4 << endl;
    cout << "----- --- R5 = " << r5 << endl;
    cout << "----- --- R6 = " << r6 << endl;
}