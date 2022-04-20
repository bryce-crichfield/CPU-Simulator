#ifndef BUS_H
#define BUS_H

#include "../def/def.hpp"

class Device;
class Bus;

enum DeviceAddress 
{
    CPU_ADDRESS = 0b00,
    RAM_ADDRESS = 0b01,
};

class Device
{
protected:
    Bus *bus;

public:
    // Device() {}
//    ~Device() = 0;

    virtual void Read()  = 0;
    virtual void Write() = 0;
    virtual void Reset() {};
    void Connect(Bus &bus, bool id);
};

class Bus
{
private:
    Device *cpu;
    Device *ram;

    sentence address_bus;
    word data_bus;

    bool read_flag;
    bool write_flag;

public:
    Bus();
    ~Bus();

    void Enroll(Device &device, bool id);
    void Notify();
    void RaiseReadFlag();
    void RaiseWriteFlag();

    sentence ReadAddressBus();
    void WriteAddressBus(sentence addr);

    word ReadDataBus();
    void WriteDataBus(word data);
};

#endif