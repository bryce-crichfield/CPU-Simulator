#ifndef CPU_H
#define CPU_H

#include "bus.hpp"

class CPU;

class CPU : public Device
{
    private:
        SysConfig* sysconfig;

        // Registers
        byte a;
        byte b; 
        byte c;
        // Control Unit
        word prc;
        word mar;
        byte cir;
        byte mdr;
        // construction register a
        word cra;
        // construction register b
        word crb;

        // Flags
        bit error;

        void(CPU::*am)();
        void(CPU::*op)();

        void PrcInc();

        void Fetch();
        void Decode();
        void Execute();

        // Immediate - load next byte in memory into register
        void IMM();
        // Direct - next 2 bytes act as pointer, load byte from there
        void DIR();
        // Indirect - next 2 bytes point to an address in memory which stores pointer
        void IND();
        // Internal - Next byte is a target register either (0x00, 0x01, 0x02)
        // Will take value at target register and place it on the MDR
        void INT();
        void NOOP();

        // Load From Memory Into Register 
        void LDA();
        void LDB();
        void LDC();
        // Store From Register to Memory
        void STA();
        void STB();
        void STC();

        // Basic Arithmetric
        void ADDA();
        void ADDB();
        void ADDC();
        void MULA();
        void MULB();
        void MULC();
        void JMP();
        void JCA();
        void JCB();
        void JCC();

    
    public:
        CPU(SysConfig& config) : sysconfig(&config) 
        { 
            a = b = c = 0;
            error = 0;
        }
        ~CPU() { }
        void Read();
        void Write(); 
        void Reset();

        void Cycle();
        void Print();
        
};


#endif