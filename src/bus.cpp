#include "../inc/bus.hpp"
#include "../inc/cpu.hpp"
#include "../inc/ram.hpp"
#include <iostream>
#include <typeinfo>

void Device::Connect(Bus& bus, bool id) 
{
    bus.Enroll(*this, id);
    this->bus = &bus;
}

Bus::Bus() 
{
    address_bus = 0;
    data_bus = 0;
    read_flag = false;
    write_flag = false;
}

Bus::~Bus() 
{
    cpu = nullptr;
    ram = nullptr;
}
void Bus::Enroll(Device& device, bool id)
{
    if (id) cpu = &device;
    else ram = &device;
    // if (typeid(Device).name() == typeid(CPU).name()) {
    //     cpu = &device;
    // } else if (typeid(Device).name() == typeid(RAM).name()) {
    //     ram = &device;
    // } else {
    //     std::cout << "Unknown Device Type: Enroll Fail" << std::endl;
    //     exit(EXIT_FAILURE);
    // }
}

// TODO: Add proper error handling for r/w
void Bus::Notify() 
{
    if (read_flag) {
        ram->Read();
        read_flag = false;
    }
    else if (write_flag) {
        ram->Write();
        write_flag = false;
    } else {
        std::cout << "Bus Notify Failure" << std::endl;
        std::cout << "No Flag Raised" << std::endl;
        exit(EXIT_FAILURE);
    }
} 

void Bus::RaiseReadFlag() 
{
    read_flag = true;
}

void Bus::RaiseWriteFlag() 
{
    write_flag = true;
}

word Bus::ReadAddressBus() 
{
    return address_bus;
}

void Bus::WriteAddressBus(word addr) 
{
    address_bus = addr;
}

byte Bus::ReadDataBus() 
{
    return data_bus;
}

void Bus::WriteDataBus(byte data)
{
    data_bus = data;
}