#ifndef RAM_H
#define RAM_H

#include "../bus/bus.hpp"
#include "../def/def.hpp"

class RAM;

class RAM : public Device 
{
    private:
        word* data_array;
        sentence current_insert;

    public:
        RAM(sentence size);
        ~RAM();

        void Read();
        void Write();
        void Reset();

        void Insert(word data);
};

#endif 