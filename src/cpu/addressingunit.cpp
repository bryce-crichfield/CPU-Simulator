#include "cpu.hpp"
#include "../bus/bus.hpp"

AddressingUnit::AddressingUnit(CPU &cpu) : SubUnit(cpu)
{
    cra = crb = 0;
    mar = mdr = 0;
}

void AddressingUnit::Fetch()
{
    mar = cpu.control_unit.prc;
    cpu.control_unit.PrcInc();
    Read();
    cpu.control_unit.cir = mdr;
}

void AddressingUnit::Read()
{
    cpu.bus->RaiseReadFlag();
    cpu.bus->WriteAddressBus(mar);
    cpu.bus->Notify();
    mdr = cpu.bus->ReadDataBus();
}

void AddressingUnit::Write()
{
    cpu.bus->RaiseWriteFlag();
    cpu.bus->WriteAddressBus(mar);
    cpu.bus->WriteDataBus(mar);
    cpu.bus->Notify();
}

void AddressingUnit::Immediate()
{
    mar = cpu.control_unit.prc;
    cpu.control_unit.PrcInc();
    Read();
}

void AddressingUnit::Direct()
{
    Immediate();
    cra = mdr;
    Immediate();
    crb = mdr;
    mar = (cra << 16) | crb;
    Read();
}

void AddressingUnit::Indirect()
{
    Direct();
    cra = mdr;
    Read();
    crb = mdr;
    mar = (cra << 16) | crb;
    Read();
}


void AddressingUnit::Print()
{
    using namespace std;
    cout << "----- Addressing Unit -----" << endl;
    cout << "----- --- MAR = 0x" << hex << mar << endl;
    cout << "----- --- MDR = 0x" << hex << mdr << endl;
}