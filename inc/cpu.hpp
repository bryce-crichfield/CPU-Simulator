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
        word stp;
        byte cir;
        byte mdr;
        word cra;
        word crb;

        void(CPU::*am)();
        void(CPU::*op)();

        void PrcInc();
        void StpInc();
        void StpDec();

        void Fetch();
        void Decode();
        void Execute();

        void Push();
        void Pop();

        void IMM();
        void DIR();
        void IND();
        void NOOP();

        void LDA();
        void LDB();
        void LDC();
        void STA();
        void STB();
        void STC();

    
    public:
        CPU(SysConfig& config) : sysconfig(&config) 
        { 
            a = 0x0;
        }
        ~CPU() { }
        void Read();
        void Write(); 
        void Reset();

        void Cycle();
        void Print();
        
};


#endif