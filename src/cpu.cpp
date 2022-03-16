#include <stdio.h>
#include <iostream>
#include "../inc/cpu.hpp"
#include "../inc/def.hpp"

// Read from the MAR, and store to the MDR
void CPU::Read()
{
    bus->RaiseReadFlag();
    bus->WriteAddressBus(mar);
    bus->Notify();
    mdr = bus->ReadDataBus();
}

// Write from the MAR and MDR-
void CPU::Write() 
{
    bus->RaiseWriteFlag();
    bus->WriteAddressBus(mar);
    bus->WriteDataBus(mdr);
    bus->Notify();
}

void CPU::Reset() {
    a = b = c = 0;
    prc = sysconfig->PRG_START;
    stp = sysconfig->STK_START;
    mar = mdr = cir = 0;
}

void CPU::Cycle() {
    Fetch();
    Decode();
    Execute();
}

void CPU::PrcInc() {
    if (prc - 1 < sysconfig->PRG_END) {
        std::cout << "PRC Overflow Dumping Core" << std::endl;
        exit(-1);
    }
    prc -= 1;
}

void CPU::StpInc() {
    if(stp + 1 > sysconfig->STK_END) {
        std::cout << "Stack Overflow Dumping Core" << std::endl;
        exit(-1);
    }
    stp += 1;
}

void CPU::StpDec() {
    if(stp - 1 < sysconfig->STK_START) {
        std::cout << "Stack Underflow Dumping Core" << std::endl;
        exit(-1);
    }
    stp -= 1;
}

void CPU::Fetch() 
{
    mar = prc;
    PrcInc();
    Read();
    cir = mdr;
}

void CPU::Decode()
{
    switch (cir)
    {
    case 0x00: am =  &CPU::IMM; op = &CPU::LDA; break;
    case 0x01: am =  &CPU::IMM; op = &CPU::LDB; break;
    case 0x02: am =  &CPU::IMM; op = &CPU::LDC; break;
    case 0x03: am =  &CPU::DIR; op = &CPU::LDA; break;
    case 0x04: am =  &CPU::DIR; op = &CPU::LDB; break;
    case 0x05: am =  &CPU::DIR; op = &CPU::LDC; break;
    case 0x06: am =  &CPU::IND; op = &CPU::LDA; break;
    case 0x07: am =  &CPU::IND; op = &CPU::LDB; break;
    case 0x08: am =  &CPU::IND; op = &CPU::LDC; break;
    case 0x0A: am =  &CPU::DIR; op = &CPU::STA; break;
    case 0x0B: am =  &CPU::DIR; op = &CPU::STB; break;
    case 0x0C: am =  &CPU::DIR; op = &CPU::STC; break;
    case 0x0D: am =  &CPU::IND; op = &CPU::STA; break;
    case 0x0E: am =  &CPU::IND; op = &CPU::STB; break;
    case 0x0F: am =  &CPU::IND; op = &CPU::STC; break;
    
    default: 
        std::cout << "Unknown Instruction" << std::endl;
        exit(-1);
    }
}

void CPU::Execute()
{
    (this->*am)();
    (this->*op)();
}

void CPU::Push()
{
    mar = stp;
    StpInc();
    Write();
}

void CPU::Pop()
{
    mar = stp;
    StpDec();
    Read();
}

void CPU::IMM() 
{
    mar = prc;
    PrcInc();
    Read();
}

void CPU::DIR()
{
    IMM();
    cra = mdr;
    IMM();
    crb = mdr;
    mar = (cra << 8) | crb;
    Read();
}

void CPU::IND()
{
    DIR();
    cra = mdr;
    Read();
    crb = mdr;
    mar = (cra << 8) | crb;
    Read();
}

void CPU::NOOP()
{
    return;
}

void CPU::LDA()
{
    a = mdr;
}

void CPU::LDB()
{
    b = mdr;
}

void CPU::LDC()
{
    c = mdr;
}

void CPU::STA()
{
    mdr = a;
    Write();
}

void CPU::STB()
{
    mdr = b;
    Write();
}

void CPU::STC()
{
    mdr = c;
    Write();
}

void CPU::Print()
{
    printf("CPU State Report: ------------------------\n");
    printf("Register A = %x\n", a);
    printf("Register B = %x\n", b);
    printf("Register C = %x\n", c);
    printf("------------------------------------------\n");
    printf("Program Counter = %x\n", prc);
    printf("Memory Address Register = %x\n", mar);
    printf("Stack Pointer = %x\n", stp);
    printf("Control Instruction Register = %x\n", cir);
    printf("Memory Data Register = %x\n", mdr);
    printf("------------------------------------------\n\n");
}