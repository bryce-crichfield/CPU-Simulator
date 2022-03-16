#ifndef RAM_H
#define RAM_H

#include "bus.hpp"
#include "def.hpp"

class RAM;

class RAM : public Device 
{
    private:
        SysConfig* sysconfig;
        byte* data_array;
        word current;

    public:
        RAM(SysConfig& config) : sysconfig(&config) 
        { 
            current = config.PRG_START;
            data_array = new byte[sysconfig->MEM_SIZE];
        }

        ~RAM() 
        {
            delete(data_array);
        }

        void Read();
        void Write();
        void Reset();

        void Insert(byte data) 
        {
            data_array[current] = data;
            current -= 1;
        }
};

#endif 