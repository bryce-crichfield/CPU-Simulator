#ifndef BUS_H
#define BUS_H

#include "def.hpp"
class Device;
class Bus;

class Device 
{
    protected:
        Bus* bus;

    public:
        Device() {}
        ~Device() {}

        virtual void Read() = 0;
        virtual void Write() = 0; 
        void Connect(Bus& bus, bool id);
        void Reset();
};    

class Bus
{
    public:
        Bus();
        ~Bus();

        void Enroll(Device& device, bool id);
        void Notify();
        void RaiseReadFlag();
        void RaiseWriteFlag();

        word ReadAddressBus();
        void WriteAddressBus(word addr);
        
        byte ReadDataBus();
        void WriteDataBus(byte data);

    private:
        Device* cpu;
        Device* ram;

        word address_bus;
        byte data_bus;

        bool read_flag;
        bool write_flag;
};

#endif