#include <stdio.h>
#include "../inc/ram.hpp"
#include  "../inc/def.hpp"

void RAM::Read()
{
    word addr = bus->ReadAddressBus();
    byte data = data_array[addr];
    bus->WriteDataBus(data);
}

void RAM::Write()
{
    word addr = bus->ReadAddressBus();
    byte data = bus->ReadDataBus();
    data_array[addr] = data;
}

void RAM::Reset()
{

}

