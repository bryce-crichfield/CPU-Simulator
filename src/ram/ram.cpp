#include <stdio.h>
#include "ram.hpp"
#include  "../def/def.hpp"

RAM::RAM(sentence size)
{ 
    data_array = new word[size];
    current_insert = 0;
}

RAM::~RAM()
{
    delete(data_array);
}

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

void RAM::Insert(word data)
{
    data_array[current_insert] = data;
    current_insert += 1;
}

